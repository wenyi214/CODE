#include "CentralCache.h"
#include "PageCache.h"

Span* CentralCache::GetOneSpan(SpanList& list, size_t size){
	Span* it = list.Begin();
	while (it != list.End()){
		
		if (it->list){
			//span�ﻹ���ڴ�
			return it;
		}
		it = it->next;
	}

	//span��û���ڴ����û��span��pagecacheҪ
	//��Ҫȷ����Ҫ�������span
	Span *sp  = pageCache.NewSpan(SizeClass::NumMovePage(size));
	//�ܴ�С
	size_t numSize = sp->n << 12;//ҳ������4096Ϊ�ܴ�С
	//�ֳ�size֮��ĸ���
	size_t n = numSize / size;
	char *start = (char *)(sp->pageid<< 12);//ҳ�ų���4096���ڵ�ַ����������ҳΪ��λ����ġ�
	sp->list = (void *)start;
	while (n){
		NextObj(start) = (start + size);
		start += size;
		n--;
	}
	return sp;

}

//����ʵ��size���ڴ�
size_t CentralCache::FetchRangeObj(void*& start, void*& end, size_t n, size_t size){
	assert(n != 0);

	size_t i = SizeClass::Index(size);
	Span* sp = GetOneSpan(spanlist[i], size);
	
	//���һ��span�ˣ��������Χ
	size_t num = 0;
	start = sp->list;
	void *prev = nullptr;
	void *cur = start;
	while (cur != nullptr&&num < n){
		prev = cur;
		cur = NextObj(cur);
		num++;
		//ȡ��һ��
		sp->use_count++;
	}
	sp->list = cur;
	end = prev;
	NextObj(end) = nullptr;

	return num;

}

void CentralCache::ReleaseListToSpans(void* start, size_t size){
	//��Ҫ֪���ڴ�����ڹ�ϣͰλ�õ��Ǹ�span�У�pagecache��map��¼��ҳ�ź�span��ӳ��
	//ͨ����ַ����ҳ�ţ�����4096���ɣ��ڴ���ҳλ��λ
	while (start){

	}
}

