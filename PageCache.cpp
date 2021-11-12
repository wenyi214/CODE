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
				Span* ps = pageSpanlist[i].PopFront();
				//�и���span
				Span* newps = new Span;
				newps->pageid = ps->pageid + pageNum;
				newps->n = ps->n - pageNum;
				//���뵽�µ�pagecache��ϣͰ��
				pageSpanlist[newps->n].PushFront(newps);
				//�Ż���Ҫ��span
				ps->n = pageNum;
				return ps;
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

	return NewSpan(pageNum);//��һ�ο϶��У��ݹ�����
}

