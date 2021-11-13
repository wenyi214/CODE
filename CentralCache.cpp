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
	
	Span *sp = pageCache.NewSpan(SizeClass::NumMovePage(size));
	//�и��span
	char *start = (char*)(sp->pageid << PAGE_SHIFT);
	char *end = start + (sp->n << PAGE_SHIFT);

	while (start < end){
		char *next = start + size;
		
		NextObj(start) = sp->list;
		sp->list = start;

		start = next;

	}

	list.PushFront(sp);

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
	size_t i = SizeClass::Index(size);
	//��Ҫ֪���ڴ�����ڹ�ϣͰλ�õ��Ǹ�span�У�pagecache��map��¼��ҳ�ź�span��ӳ��
	//ͨ����ַ����ҳ�ţ�����4096���ɣ��ڴ���ҳλ��λ
	while (start){
		//���ҳ��
		//���span
		Span* sp = pageCache.ObjToSpan(start);

		//����start����
		void *next = NextObj(start);
		//ͷ��
		NextObj(start) = sp->list;
		sp->list = start;
		sp->use_count--;
		//span�е�С���ڴ�ȫ�����أ���Ҫ������pagecache��
		if (sp->use_count == 0){
			//��������ɾ��
			spanlist[i].Pop(sp);
			sp->list = nullptr;

			//���뵽pagecahe��
			pageCache.ReleaseSpanToPageCache(sp);
		}

		start = next;

	}
}

