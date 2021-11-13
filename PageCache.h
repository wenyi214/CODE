#pragma once 

#include "Comment.h"


class PageCache{
private:
	SpanList pageSpanlist[NUMPAGES];//按页数映射，0不用
	std::map<PageID, Span*> IsSpanMap;//页号映射Span，更好查找Span

public:


	//向系统申请内存
	void* SystemAllocPage(size_t size);	
	//返还一个span给centralcache
	Span* NewSpan(size_t pageNum);

	Span *ObjToSpan(void *start);

	void ReleaseSpanToPageCache(Span* sp);

};

static PageCache pageCache;