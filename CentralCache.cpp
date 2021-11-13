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
	
	Span *sp = pageCache.NewSpan(SizeClass::NumMovePage(size));
	//切割号span
	char *start = (char*)(sp->pageid << PAGE_SHIFT);
	char *end = start + (sp->n << PAGE_SHIFT);

	while (start < end){
		char *next = start + size;
		
		NextObj(start) = sp->list;
		sp->list = start;

		start = next;

	}

	list.PushFront(sp);

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
	size_t i = SizeClass::Index(size);
	//需要知道内存块属于哈希桶位置的那个span中，pagecache用map记录了页号和span的映射
	//通过地址计算页号，除以4096即可，内存以页位单位
	while (start){
		//获得页号
		//获得span
		Span* sp = pageCache.ObjToSpan(start);

		//方便start更新
		void *next = NextObj(start);
		//头插
		NextObj(start) = sp->list;
		sp->list = start;
		sp->use_count--;
		//span中的小块内存全部返回，需要返还到pagecache中
		if (sp->use_count == 0){
			//从链表中删除
			spanlist[i].Pop(sp);
			sp->list = nullptr;

			//插入到pagecahe中
			pageCache.ReleaseSpanToPageCache(sp);
		}

		start = next;

	}
}

