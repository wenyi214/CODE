#pragma once
#include "Comment.h"



//结构为一个哈希桶，桶里连接的大块内存
class CentralCache{
private:
	SpanList spanlist[NFREELISTS];//按span中的size映射

public:
	size_t FetchRangeObj(void*& start, void*& end, size_t n, size_t size);
	Span* GetOneSpan(SpanList& list, size_t size);

	void ReleaseListToSpans(void* start, size_t size);
};

static CentralCache centralCache;