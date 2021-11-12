#pragma once
#include "Comment.h"



//�ṹΪһ����ϣͰ��Ͱ�����ӵĴ���ڴ�
class CentralCache{
private:
	SpanList spanlist[NFREELISTS];//��span�е�sizeӳ��

public:
	size_t FetchRangeObj(void*& start, void*& end, size_t n, size_t size);
	Span* GetOneSpan(SpanList& list, size_t size);

	void ReleaseListToSpans(void* start, size_t size);
};

static CentralCache centralCache;