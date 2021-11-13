#pragma once 

#include "Comment.h"


class PageCache{
private:
	SpanList pageSpanlist[NUMPAGES];//��ҳ��ӳ�䣬0����
	std::map<PageID, Span*> IsSpanMap;//ҳ��ӳ��Span�����ò���Span

public:


	//��ϵͳ�����ڴ�
	void* SystemAllocPage(size_t size);	
	//����һ��span��centralcache
	Span* NewSpan(size_t pageNum);

	Span *ObjToSpan(void *start);

	void ReleaseSpanToPageCache(Span* sp);

};

static PageCache pageCache;