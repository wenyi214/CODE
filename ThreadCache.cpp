#include "ThreadCache.h"
#include "CentralCache.h"

//从centralcache里申请空间
void* ThreadCache::FetchFromCentral(size_t size){
	//批量申请，防止频繁向centralcache申请空间，共享资源
	//慢启动
	size_t pos = SizeClass::Index(size);
	//批量向centralcache申请内存块的个数，
	size_t num = min(SizeClass::NumMoveSize(size), freelist[pos].GetMaxsize());

	//当maxsize == num时，maxsize才增加，否则取的是NumMoveSize的个数，增加没意义
	if (num == freelist[pos].GetMaxsize()){
		freelist[pos].SetMaxsize(freelist[pos].GetMaxsize() + 1);
	}

	void *start = nullptr;
	void *end = nullptr;
	//申请num个size大小的内存块，返回直接centralcache给的个数，到末尾可能不够，但是有一个就可以了
	//通过start end来接收内存块的首尾，
	size_t actualNum = centralCache.FetchRangeObj(start, end, num, size);
	assert(actualNum != 0);
	 
	if (actualNum > 1){
		//返回个数大于1，将一个返回，其它内存连接到threadcache对尾链表中
		freelist[pos].PushRange(NextObj(start), end, actualNum);
	}
	
	//central只有一个，直接返回
	if (actualNum == 1){

	}
	return start;

}
//用户申请内存
void* ThreadCache::Allocate(size_t size){

	size_t i = SizeClass::Index(size);
	if (!freelist[i].Empty()){
		return freelist[i].Pop();
	}
	
	//向centralpage申请
	return FetchFromCentral(size);

}
void ThreadCache::ListTooLong(FreeList list, size_t size){
	//要还给central cache的个数
	size_t actul_num = list.GetMaxsize();

	void *start = nullptr;
	void *end = nullptr;
	//从freelist中获取n个内存块
	list.PopRange(start, end, actul_num);

	centralCache.ReleaseListToSpans(start, actul_num);

}

//后面要取消size
//用户释放内存
void ThreadCache::Deallocate(void *ptr, size_t size){
	size_t i = SizeClass::Index(size);
	freelist[i].Push(ptr);
	//改进，防止哈希桶中太多的内存块，需要交给central cache
	//释放不是固定一个值，maxsize记录了centralcache给threadcache内存块个数的最大值
	if (freelist[i].GetSize() > freelist[i].GetMaxsize()){
		ListTooLong(freelist[i], size);
 	}
}


