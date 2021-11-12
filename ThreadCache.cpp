#include "ThreadCache.h"
#include "CentralCache.h"

//��centralcache������ռ�
void* ThreadCache::FetchFromCentral(size_t size){
	//�������룬��ֹƵ����centralcache����ռ䣬������Դ
	//������
	size_t pos = SizeClass::Index(size);
	//������centralcache�����ڴ��ĸ�����
	size_t num = min(SizeClass::NumMoveSize(size), freelist[pos].GetMaxsize());

	//��maxsize == numʱ��maxsize�����ӣ�����ȡ����NumMoveSize�ĸ���������û����
	if (num == freelist[pos].GetMaxsize()){
		freelist[pos].SetMaxsize(freelist[pos].GetMaxsize() + 1);
	}

	void *start = nullptr;
	void *end = nullptr;
	//����num��size��С���ڴ�飬����ֱ��centralcache���ĸ�������ĩβ���ܲ�����������һ���Ϳ�����
	//ͨ��start end�������ڴ�����β��
	size_t actualNum = centralCache.FetchRangeObj(start, end, num, size);
	assert(actualNum != 0);
	 
	if (actualNum > 1){
		//���ظ�������1����һ�����أ������ڴ����ӵ�threadcache��β������
		freelist[pos].PushRange(NextObj(start), end, actualNum);
	}
	
	//centralֻ��һ����ֱ�ӷ���
	if (actualNum == 1){

	}
	return start;

}
//�û������ڴ�
void* ThreadCache::Allocate(size_t size){

	size_t i = SizeClass::Index(size);
	if (!freelist[i].Empty()){
		return freelist[i].Pop();
	}
	
	//��centralpage����
	return FetchFromCentral(size);

}
void ThreadCache::ListTooLong(FreeList list, size_t size){
	//Ҫ����central cache�ĸ���
	size_t actul_num = list.GetMaxsize();

	void *start = nullptr;
	void *end = nullptr;
	//��freelist�л�ȡn���ڴ��
	list.PopRange(start, end, actul_num);

	centralCache.ReleaseListToSpans(start, actul_num);

}

//����Ҫȡ��size
//�û��ͷ��ڴ�
void ThreadCache::Deallocate(void *ptr, size_t size){
	size_t i = SizeClass::Index(size);
	freelist[i].Push(ptr);
	//�Ľ�����ֹ��ϣͰ��̫����ڴ�飬��Ҫ����central cache
	//�ͷŲ��ǹ̶�һ��ֵ��maxsize��¼��centralcache��threadcache�ڴ����������ֵ
	if (freelist[i].GetSize() > freelist[i].GetMaxsize()){
		ListTooLong(freelist[i], size);
 	}
}


