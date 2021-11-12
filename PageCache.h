#pragma once 

#include "Comment.h"


class PageCache{
private:
	SpanList pageSpanlist[NUMPAGES];//按页数映射，0不用

	std::map<PageID, Span*> isSpanMap;//页号映射Span，更好查找Span
	
public:
	void* SystemAllocPage(size_t size);	
	Span* NewSpan(size_t pageNum);

};

static PageCache pageCache;