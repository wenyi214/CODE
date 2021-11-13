#include "PageCache.h"


void* PageCache::SystemAllocPage(size_t size){

	return SystemAlloc(size);
}



Span* PageCache::NewSpan(size_t pageNum){
	if (!(pageSpanlist[pageNum].IsEmpty())){
		return pageSpanlist[pageNum].PopFront();
	}
	//����Ҵ��page
	for (int i = pageNum + 1; i < NUMPAGES; i++){
		if (!pageSpanlist[i].IsEmpty()){
			//�и���ӵ���λ�ã�������Ҫ��
			if (!(pageSpanlist[i].IsEmpty())){
				//β��Ч�ʸߣ�һ����ҪС����ڴ棬����ӳ���ϵ��
				Span *sp = pageSpanlist[i].PopFront();
				sp->n = sp->n - pageNum;
				Span* newsp = new Span;
				newsp->pageid = sp->pageid + sp->n;
				newsp->n = pageNum;
				//����ӳ���ϵ
				for (size_t i = 0; i < newsp->n; i++){
					IsSpanMap[newsp->pageid + i] = newsp;
				}
				//���뵽�µ�pagecache��ϣͰ��
				pageSpanlist[sp->n].PushFront(sp);
				return newsp;


				//ͷ��
				//Span* ps = pageSpanlist[i].PopFront();
				////�и���span
				//Span* newsp = new Span;
				//newsp->pageid = ps->pageid + pageNum;
				//newsp->n = ps->n - pageNum;
				//for (int i = 0; i < newsp->n; i++){
				//	IsSpanMap[newsp->pageid + i] = newsp;
				//}
				////���뵽�µ�pagecache��ϣͰ��
				//pageSpanlist[newsp->n].PushFront(newsp);
				////�Ż���Ҫ��span
				//ps->n = pageNum;
				//return ps;
			}
		}
	}

	//˵��pageSpanlistû�У���Ҫ����
	Span* sp = new Span;
	void *memory = SystemAllocPage(NUMPAGES - 1);//ֱ������һ������
	//ע��дһ��,����ͨ��ҳ�����Ƶ���ָ��
	sp->pageid = (size_t)memory >> 12;//ָ���Ƕ��ֽڵı�ţ�����ҳ�ĵ�λ����֪�������Ǹ�ҳ
	sp->n = NUMPAGES - 1;//ҳ��
	pageSpanlist[NUMPAGES - 1].PushFront(sp);

	for (size_t i = 0; i < sp->n; i++){
		IsSpanMap[sp->pageid + i] = sp;
	}

	return NewSpan(pageNum);//��һ�ο϶��У��ݹ�����
}
Span *PageCache::ObjToSpan(void *start){
	PageID id = (size_t)start >> PAGE_SHIFT;

	return IsSpanMap[id];
}



//��span����pagecache��
void PageCache::ReleaseSpanToPageCache(Span* span){
	//��ǰ��ĺϲ�
	while (1){
		PageID id = span->pageid - 1;
		if (IsSpanMap.find(id) == IsSpanMap.end()){
			break;
		}

		Span* prevSpan = IsSpanMap[id];

		if (prevSpan->use_count != 0){
			break;
		}

		//Span�����Ҷ������ˣ����кϲ�
		//��ȡ��
		pageSpanlist[prevSpan->n].Pop(prevSpan);
		//�ϲ�
		span->pageid = prevSpan->pageid;
		span->n += prevSpan->n;

		//�޸�mapӳ���ϵ��
		for (size_t i = 0; i < prevSpan->n; i++){
			IsSpanMap[prevSpan->pageid + i] = span;
		}

		delete prevSpan;
	}

	//�Һ���ĺϲ�
	while (1){
		PageID id = (size_t)span->list >> PAGE_SHIFT;
		if (IsSpanMap.find(id + span->n) == IsSpanMap.end()){
			break;
		}

		Span *nextSpan = IsSpanMap[id + span->n];

		if (nextSpan->use_count != 0){
			break;
		}
		//ȡ��
		pageSpanlist[nextSpan->n].Pop(nextSpan);

		span->n += nextSpan->n;

		for (size_t i = 0; i < nextSpan->n; i++){
			IsSpanMap[nextSpan->pageid + i] = span;
		}

		delete nextSpan;
	}
	//�ϲ����span���뵽��ϣͰ��
	pageSpanlist[span->n].PushFront(span);

}

