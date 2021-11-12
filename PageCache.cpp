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
				Span* ps = pageSpanlist[i].PopFront();
				//切割大的span
				Span* newps = new Span;
				newps->pageid = ps->pageid + pageNum;
				newps->n = ps->n - pageNum;
				//插入到新的pagecache哈希桶中
				pageSpanlist[newps->n].PushFront(newps);
				//放回需要的span
				ps->n = pageNum;
				return ps;
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

	return NewSpan(pageNum);//下一次肯定有，递归申请
}

