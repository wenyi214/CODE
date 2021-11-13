#pragma once
#include "Comment.h"



//结构为一个哈希桶，桶里连接的大块内存
class CentralCache{
private:
	SpanList spanlist[NFREELISTS];//按span中的size映射

public:
	//分配多个内存块给threadcache
	size_t FetchRangeObj(void*& start, void*& end, size_t n, size_t size);
	//获得一个span，从哈希桶位置或者pagecache中
	Span* GetOneSpan(SpanList& list, size_t size);
	//回收span到span中
	void ReleaseListToSpans(void* start, size_t size);
};

static CentralCache centralCache;