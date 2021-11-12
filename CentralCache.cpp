#include "CentralCache.h"
#include "PageCache.h"

Span* CentralCache::GetOneSpan(SpanList& list, size_t size){
	Span* it = list.Begin();
	while (it != list.End()){
		
		if (it->list){
			//span里还有内存
			return it;
		}
		it = it->next;
	}

	//span中没有内存或者没有span找pagecache要
	//需要确认需要申请多大的span
	Span *sp  = pageCache.NewSpan(SizeClass::NumMovePage(size));
	//总大小
	size_t numSize = sp->n << 12;//页数成以4096为总大小
	//分成size之后的个数
	size_t n = numSize / size;
	char *start = (char *)(sp->pageid<< 12);//页号乘以4096等于地址，申请是以页为单位申请的。
	sp->list = (void *)start;
	while (n){
		NextObj(start) = (start + size);
		start += size;
		n--;
	}
	return sp;

}

//返回实际size个内存
size_t CentralCache::FetchRangeObj(void*& start, void*& end, size_t n, size_t size){
	assert(n != 0);

	size_t i = SizeClass::Index(size);
	Span* sp = GetOneSpan(spanlist[i], size);
	
	//获得一个span了，获得链表范围
	size_t num = 0;
	start = sp->list;
	void *prev = nullptr;
	void *cur = start;
	while (cur != nullptr&&num < n){
		prev = cur;
		cur = NextObj(cur);
		num++;
		//取走一个
		sp->use_count++;
	}
	sp->list = cur;
	end = prev;
	NextObj(end) = nullptr;

	return num;

}

void CentralCache::ReleaseListToSpans(void* start, size_t size){
	//需要知道内存块属于哈希桶位置的那个span中，pagecache用map记录了页号和span的映射
	//通过地址结算页号，除以4096即可，内存以页位单位
	while (start){

	}
}

