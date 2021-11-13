#include "PageCache.h"


void* PageCache::SystemAllocPage(size_t size){

	return SystemAlloc(size);
}



Span* PageCache::NewSpan(size_t pageNum){
	if (!(pageSpanlist[pageNum].IsEmpty())){
		return pageSpanlist[pageNum].PopFront();
	}
	//向后找大的page
	for (int i = pageNum + 1; i < NUMPAGES; i++){
		if (!pageSpanlist[i].IsEmpty()){
			//切割，连接到新位置，返回需要的
			if (!(pageSpanlist[i].IsEmpty())){
				//尾切效率高，一般需要小块的内存，更新映射关系少
				Span *sp = pageSpanlist[i].PopFront();
				sp->n = sp->n - pageNum;
				Span* newsp = new Span;
				newsp->pageid = sp->pageid + sp->n;
				newsp->n = pageNum;
				//更新映射关系
				for (size_t i = 0; i < newsp->n; i++){
					IsSpanMap[newsp->pageid + i] = newsp;
				}
				//插入到新的pagecache哈希桶中
				pageSpanlist[sp->n].PushFront(sp);
				return newsp;


				//头切
				//Span* ps = pageSpanlist[i].PopFront();
				////切割大的span
				//Span* newsp = new Span;
				//newsp->pageid = ps->pageid + pageNum;
				//newsp->n = ps->n - pageNum;
				//for (int i = 0; i < newsp->n; i++){
				//	IsSpanMap[newsp->pageid + i] = newsp;
				//}
				////插入到新的pagecache哈希桶中
				//pageSpanlist[newsp->n].PushFront(newsp);
				////放回需要的span
				//ps->n = pageNum;
				//return ps;
			}
		}
	}

	//说明pageSpanlist没有，需要申请
	Span* sp = new Span;
	void *memory = SystemAllocPage(NUMPAGES - 1);//直接申请一个最大的
	//注意写一下,可以通过页号来推导出指针
	sp->pageid = (size_t)memory >> 12;//指针是对字节的编号，除以页的单位，就知道属于那个页
	sp->n = NUMPAGES - 1;//页数
	pageSpanlist[NUMPAGES - 1].PushFront(sp);

	for (size_t i = 0; i < sp->n; i++){
		IsSpanMap[sp->pageid + i] = sp;
	}

	return NewSpan(pageNum);//下一次肯定有，递归申请
}
Span *PageCache::ObjToSpan(void *start){
	PageID id = (size_t)start >> PAGE_SHIFT;

	return IsSpanMap[id];
}



//将span插入pagecache中
void PageCache::ReleaseSpanToPageCache(Span* span){
	//找前面的合并
	while (1){
		PageID id = span->pageid - 1;
		if (IsSpanMap.find(id) == IsSpanMap.end()){
			break;
		}

		Span* prevSpan = IsSpanMap[id];

		if (prevSpan->use_count != 0){
			break;
		}

		//Span存在且都回来了，进行合并
		//先取出
		pageSpanlist[prevSpan->n].Pop(prevSpan);
		//合并
		span->pageid = prevSpan->pageid;
		span->n += prevSpan->n;

		//修改map映射关系、
		for (size_t i = 0; i < prevSpan->n; i++){
			IsSpanMap[prevSpan->pageid + i] = span;
		}

		delete prevSpan;
	}

	//找后面的合并
	while (1){
		PageID id = (size_t)span->list >> PAGE_SHIFT;
		if (IsSpanMap.find(id + span->n) == IsSpanMap.end()){
			break;
		}

		Span *nextSpan = IsSpanMap[id + span->n];

		if (nextSpan->use_count != 0){
			break;
		}
		//取出
		pageSpanlist[nextSpan->n].Pop(nextSpan);

		span->n += nextSpan->n;

		for (size_t i = 0; i < nextSpan->n; i++){
			IsSpanMap[nextSpan->pageid + i] = span;
		}

		delete nextSpan;
	}
	//合并大的span插入到哈希桶中
	pageSpanlist[span->n].PushFront(span);

}

