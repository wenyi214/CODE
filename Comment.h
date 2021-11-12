#pragma once

#include <iostream>
#include <map>
#include <exception>
#include <time.h>
#include <windows.h>
#include <assert.h>
#include <algorithm>
#include <windows.h>

static const size_t MAX_BYTES = 64 * 1024; //threadcache哈希桶个数，大于64k，从pagecache申请
static const size_t NFREELISTS = 184;//区间映射关系不同，总数需要改变
static const size_t NUMPAGES = 129;	
static const size_t PAGE_SHIFT = 12;//32位系统，页的单位4kb


//获取链表下一个obj，用内存空间保存
static inline void*& NextObj(void* obj){
	//返回前面指针大小
	return *((void**)obj);
}

//向系统堆上申请空间
static inline void* SystemAlloc(size_t kpage){
#ifdef _WIN32
	void *ptr = VirtualAlloc(0, kpage*(1 << PAGE_SHIFT), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
	//brk mmap
#endif
	//抛异常
	if (ptr == nullptr){
		throw std::bad_alloc();
	}
	return ptr;
}
//释放空间
static inline void SystemFree(void *ptr){
#ifdef _WIN32
	VirtualFree(ptr,0,MEM_RELEASE);
#else
	//sbrk unmmap
#endif

}



class SizeClass{
	//防止由于对齐数太小导致threadcache哈希桶太大，将哈希桶划分多个区间，每个区间对齐数不同。如下：
	//[1, 128]字节，			对齐数8，		区间个数为[0, 15]		16个
	//[129, 1024]字节，			对齐数16，		区间个数为[16, 71]		56个
	//[1025, 8*1024]字节，		对齐数128，		区间个数为[72, 127]		56个 
	//[8*1024+1, 64*1024]字节，	对齐数1024，	区间个数为[128, 183]	56个
private:
	static inline size_t _Index(size_t bitNum, size_t n){
		//n为对齐数
		//(1<<n)对齐数，加上(1<<n)-1防止bitNum出现小于区间的值，>>n除以对齐数
		//正好为对齐数倍数的 8/8-1正好，小于对齐数倍数的，7/8正好
		//如果直接除以对齐数，5/8-1为负数，越界了。
		//加上对齐数减1，不是对齐数倍数的会到下一个对齐数倍数，并且正好为对齐数倍数的也不会为下一个倍数。
		//统一了求位置的方式
		
		return ((bitNum + ((1 << n) - 1)) >> n) - 1;
	}
public:
	//获取threadcache哈希桶的位置
	static size_t Index(size_t size){
		assert(size <= MAX_BYTES);
		//哈希桶区间个数
		size_t RangeNum[] = { 16, 56, 56, 56 };
		//计算出每个区间距离区间起始位置的距离，加上之前的区间个数，就是当前位置
		//所以需要减去前面区间的数，前面区间数对齐数不同
		if (size <= 128){
			return _Index(size - 0, 3);
		}
		else if (size <= 1024){
			return _Index(size - 128, 4) + RangeNum[0];
		}
		else if (size <= 8 * 1024){
			return _Index(size - 1024, 7) + RangeNum[1] + RangeNum[0];
		}
		else if (size <= 64 * 1024){
			return _Index(size - 8 * 1024, 10) + RangeNum[2] + RangeNum[1] + RangeNum[0];
		}
		else{

		}
		return -1;
	}

	//从centralcache获取内存块，可能的个数
	static size_t NumMoveSize(size_t size){

		//用最大字节数除申请的字节数
		size_t num = MAX_BYTES / size;
		if (num == 0){
			return 2;
		}

		if (num > 512){
			return 512;
		}

		return num;
	}
	//返回页数
	static size_t NumMovePage(size_t size){
		//centralcache分配给threadcache多少内存块
		int num = NumMoveSize(size);
		//最多需要多少字节内存
		int numSize = num * size;

		//页数
		int pageNum = numSize >> 12;//除以4kb
		//除以等于0，至少分一页
		if (pageNum == 0){
			return 1;
		}

		return pageNum;
	}


};


class FreeList{
private:
	void *head;//链表头指针
	size_t maxsize;
	size_t size;//链表长度
public:
	FreeList()
		:head(nullptr)
		,maxsize(1)
		,size(0)
	{}
	size_t GetSize(){
		return size;
	}
	size_t GetMaxsize(){
		return maxsize;
	}
	void SetMaxsize(size_t msize){
		maxsize = msize;
	}

	//将多个centralcache给的内存连接到链表中
	void PushRange(void* start, void* end, size_t n){
		NextObj(end) = head;
		head = start;
		size += n;
		
	}
	void PopRange(void*& start, void*& end, size_t n){
		//不可能会不够
		assert(n != 0);
		start = head;
		void *cur = head;
		size_t i = 1;
		while (i < n){
			cur = NextObj(cur);
			i++;
		}

		head = NextObj(cur);
		end = cur;

	}

	void Push(void *obj){
		//头插
		//obj指向下一个内存，头指针指向obj
		NextObj(obj) = head;
		head = obj;
		size++;
	}

	void *Pop(){
		//头删
		void *obj = head;
		head = NextObj(obj);
		return obj;
		size--;
	}
	bool Empty(){
		return head == nullptr;
	}

	~FreeList()
	{}
};

typedef size_t PageID;//为了后面映射span用
//以页为单位的大块内存
struct Span{
	
	//为pagecache合并用
	PageID pageid = 0;//第一个页号
	size_t n = 0;//有多少个页

	Span *next = nullptr;//将Span连接起来
	Span *prev = nullptr;

	size_t size = 0;//只针对切割相同大小为size的内存
				//不同central哈希桶下size不同

	void *list = nullptr;//大块内存
	size_t use_count = 0;//大块内存分割个数,0代表没有分割
};
//用链表将相同大小Span(大块内存)管理起来
//为什么带头双向链表，如果哪一个大块内存全部未使用，pagecache需要回收，组合成大的页，方便删除
class SpanList{
private:
	Span *head;
public:
	SpanList(){
		head = new Span;

		head->next = head;
		head->prev = head;
	}
	//在pos位置前插入
	void Push(Span* pos, Span *sp){
		Span* posprev = pos->prev;

		sp->prev = posprev;
		posprev->next = sp;

		sp->next = pos;
		pos->prev = sp;

	}
	//删除pos位置Span
	Span* Pop(Span* pos){
		assert(pos != head);
		Span* posprev = pos->prev;
		Span* posnext = pos->next;

		posprev->next = posnext;
		posnext->prev = posprev;

	}

	//头插
	void PushFront(Span *sp){
		Span* spannext = head->next;
		head->next = sp;
		sp->prev = head;

		sp->next = spannext;
		spannext->prev = sp;
	}
	//头删
	Span* PopFront(){
		Span* sp = head->next;
		Span* nnext = sp->next;
		head->next = nnext;
		nnext->prev = head;

		return sp;
	}
	bool IsEmpty(){
		return head->next == head;
	}
	//为了遍历
	Span* Begin(){
		return head->next;
	}
	Span* End(){
		return head;
	}
};







//如果以8作为间隔，每个位置代表8的整数倍
//如果直接返回size / 8 - 1结果不对。第一：size小于8，为负数，第二：size不是8的整数倍结果错误



// size + 8-1 如果size小于等于8，即[1,NUM]提升到[NUM,2*NUM-1]，不会为负数
//整数倍加8-1也不会到下一个整数倍，除8，结果还是原来值，再减1，结果正确
//不是整数倍加8-1，正好向上取到整数倍，除NUM，再减1，再减1，结果正确
//static size_t GetIndex(size_t size){
//	return ((size + (8 - 1)) >> 3) - 1;
//}
//传统写法
//static size_t GetIndex1(size_t size){
//	if (size % 8){  //可以整除NUM
//		return size / 8 - 1;
//	}
//	else{
//		return size / 8;
//	}
//}