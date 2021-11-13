#pragma once
#include "Comment.h"



//�ṹΪһ����ϣͰ��Ͱ�����ӵĴ���ڴ�
class CentralCache{
private:
	SpanList spanlist[NFREELISTS];//��span�е�sizeӳ��

public:
	//�������ڴ���threadcache
	size_t FetchRangeObj(void*& start, void*& end, size_t n, size_t size);
	//���һ��span���ӹ�ϣͰλ�û���pagecache��
	Span* GetOneSpan(SpanList& list, size_t size);
	//����span��span��
	void ReleaseListToSpans(void* start, size_t size);
};

static CentralCache centralCache;