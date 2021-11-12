#pragma once 

#include "Comment.h"


class PageCache{
private:
	SpanList pageSpanlist[NUMPAGES];//��ҳ��ӳ�䣬0����

	std::map<PageID, Span*> isSpanMap;//ҳ��ӳ��Span�����ò���Span
	
public:
	void* SystemAllocPage(size_t size);	
	Span* NewSpan(size_t pageNum);

};

static PageCache pageCache;