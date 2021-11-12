#pragma once

#include <iostream>
#include <map>
#include <exception>
#include <time.h>
#include <windows.h>
#include <assert.h>
#include <algorithm>
#include <windows.h>

static const size_t MAX_BYTES = 64 * 1024; //threadcache��ϣͰ����������64k����pagecache����
static const size_t NFREELISTS = 184;//����ӳ���ϵ��ͬ��������Ҫ�ı�
static const size_t NUMPAGES = 129;	
static const size_t PAGE_SHIFT = 12;//32λϵͳ��ҳ�ĵ�λ4kb


//��ȡ������һ��obj�����ڴ�ռ䱣��
static inline void*& NextObj(void* obj){
	//����ǰ��ָ���С
	return *((void**)obj);
}

//��ϵͳ��������ռ�
static inline void* SystemAlloc(size_t kpage){
#ifdef _WIN32
	void *ptr = VirtualAlloc(0, kpage*(1 << PAGE_SHIFT), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
	//brk mmap
#endif
	//���쳣
	if (ptr == nullptr){
		throw std::bad_alloc();
	}
	return ptr;
}
//�ͷſռ�
static inline void SystemFree(void *ptr){
#ifdef _WIN32
	VirtualFree(ptr,0,MEM_RELEASE);
#else
	//sbrk unmmap
#endif

}



class SizeClass{
	//��ֹ���ڶ�����̫С����threadcache��ϣͰ̫�󣬽���ϣͰ���ֶ�����䣬ÿ�������������ͬ�����£�
	//[1, 128]�ֽڣ�			������8��		�������Ϊ[0, 15]		16��
	//[129, 1024]�ֽڣ�			������16��		�������Ϊ[16, 71]		56��
	//[1025, 8*1024]�ֽڣ�		������128��		�������Ϊ[72, 127]		56�� 
	//[8*1024+1, 64*1024]�ֽڣ�	������1024��	�������Ϊ[128, 183]	56��
private:
	static inline size_t _Index(size_t bitNum, size_t n){
		//nΪ������
		//(1<<n)������������(1<<n)-1��ֹbitNum����С�������ֵ��>>n���Զ�����
		//����Ϊ������������ 8/8-1���ã�С�ڶ����������ģ�7/8����
		//���ֱ�ӳ��Զ�������5/8-1Ϊ������Խ���ˡ�
		//���϶�������1�����Ƕ����������Ļᵽ��һ����������������������Ϊ������������Ҳ����Ϊ��һ��������
		//ͳһ����λ�õķ�ʽ
		
		return ((bitNum + ((1 << n) - 1)) >> n) - 1;
	}
public:
	//��ȡthreadcache��ϣͰ��λ��
	static size_t Index(size_t size){
		assert(size <= MAX_BYTES);
		//��ϣͰ�������
		size_t RangeNum[] = { 16, 56, 56, 56 };
		//�����ÿ���������������ʼλ�õľ��룬����֮ǰ��������������ǵ�ǰλ��
		//������Ҫ��ȥǰ�����������ǰ����������������ͬ
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

	//��centralcache��ȡ�ڴ�飬���ܵĸ���
	static size_t NumMoveSize(size_t size){

		//������ֽ�����������ֽ���
		size_t num = MAX_BYTES / size;
		if (num == 0){
			return 2;
		}

		if (num > 512){
			return 512;
		}

		return num;
	}
	//����ҳ��
	static size_t NumMovePage(size_t size){
		//centralcache�����threadcache�����ڴ��
		int num = NumMoveSize(size);
		//�����Ҫ�����ֽ��ڴ�
		int numSize = num * size;

		//ҳ��
		int pageNum = numSize >> 12;//����4kb
		//���Ե���0�����ٷ�һҳ
		if (pageNum == 0){
			return 1;
		}

		return pageNum;
	}


};


class FreeList{
private:
	void *head;//����ͷָ��
	size_t maxsize;
	size_t size;//������
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

	//�����centralcache�����ڴ����ӵ�������
	void PushRange(void* start, void* end, size_t n){
		NextObj(end) = head;
		head = start;
		size += n;
		
	}
	void PopRange(void*& start, void*& end, size_t n){
		//�����ܻ᲻��
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
		//ͷ��
		//objָ����һ���ڴ棬ͷָ��ָ��obj
		NextObj(obj) = head;
		head = obj;
		size++;
	}

	void *Pop(){
		//ͷɾ
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

typedef size_t PageID;//Ϊ�˺���ӳ��span��
//��ҳΪ��λ�Ĵ���ڴ�
struct Span{
	
	//Ϊpagecache�ϲ���
	PageID pageid = 0;//��һ��ҳ��
	size_t n = 0;//�ж��ٸ�ҳ

	Span *next = nullptr;//��Span��������
	Span *prev = nullptr;

	size_t size = 0;//ֻ����и���ͬ��СΪsize���ڴ�
				//��ͬcentral��ϣͰ��size��ͬ

	void *list = nullptr;//����ڴ�
	size_t use_count = 0;//����ڴ�ָ����,0����û�зָ�
};
//��������ͬ��СSpan(����ڴ�)��������
//Ϊʲô��ͷ˫�����������һ������ڴ�ȫ��δʹ�ã�pagecache��Ҫ���գ���ϳɴ��ҳ������ɾ��
class SpanList{
private:
	Span *head;
public:
	SpanList(){
		head = new Span;

		head->next = head;
		head->prev = head;
	}
	//��posλ��ǰ����
	void Push(Span* pos, Span *sp){
		Span* posprev = pos->prev;

		sp->prev = posprev;
		posprev->next = sp;

		sp->next = pos;
		pos->prev = sp;

	}
	//ɾ��posλ��Span
	Span* Pop(Span* pos){
		assert(pos != head);
		Span* posprev = pos->prev;
		Span* posnext = pos->next;

		posprev->next = posnext;
		posnext->prev = posprev;

	}

	//ͷ��
	void PushFront(Span *sp){
		Span* spannext = head->next;
		head->next = sp;
		sp->prev = head;

		sp->next = spannext;
		spannext->prev = sp;
	}
	//ͷɾ
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
	//Ϊ�˱���
	Span* Begin(){
		return head->next;
	}
	Span* End(){
		return head;
	}
};







//�����8��Ϊ�����ÿ��λ�ô���8��������
//���ֱ�ӷ���size / 8 - 1������ԡ���һ��sizeС��8��Ϊ�������ڶ���size����8���������������



// size + 8-1 ���sizeС�ڵ���8����[1,NUM]������[NUM,2*NUM-1]������Ϊ����
//��������8-1Ҳ���ᵽ��һ������������8���������ԭ��ֵ���ټ�1�������ȷ
//������������8-1����������ȡ������������NUM���ټ�1���ټ�1�������ȷ
//static size_t GetIndex(size_t size){
//	return ((size + (8 - 1)) >> 3) - 1;
//}
//��ͳд��
//static size_t GetIndex1(size_t size){
//	if (size % 8){  //��������NUM
//		return size / 8 - 1;
//	}
//	else{
//		return size / 8;
//	}
//}