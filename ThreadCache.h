#pragma once

#include "Comment.h"

//�ṹΪһ����ϣͰ��Ͱ�����ӵ�С���ڴ�
class ThreadCache{
private:
	//�ṹ�����飬�ṹ�������һ����������ͷָ��
	FreeList freelist[MAX_BYTES];//��������ӳ��
public:
	ThreadCache()
	{}
	//����̫������Ҫ�ϲ���centralcache��span��
	void ListTooLong(FreeList list, size_t size);

	//�����Ͷ������
	//�����ڴ�
	void *Allocate(size_t size);
	//�����ڴ�
	void Deallocate(void *ptr, size_t size);
	//��centralcache�����ڴ�
	void* FetchFromCentral(size_t size);

	~ThreadCache()
	{}

};
//��ô����Thread Cache�߳�˽�У�TLS
//TLS thread local storage�̱߳��ش洢
//һ��һ��ȫ�ֱ����������TLS���͵ģ���ôÿ���̶߳���ӵ�����������һ���������κ��̶߳Ըñ������޸Ķ�����Ӱ�������߳��иñ����ĸ�����
// ��ʹ��_declspec(thread)����һ���߳�˽�б���ʱ�������������Щ�����ŵ�PE�ļ��ġ�.tls"���С�
//��ϵͳ����һ���µ��߳�ʱ������ӽ��̵Ķ��з���һ���㹻��С�Ŀռ䣬Ȼ��ѡ�.tls"���е����ݸ��Ƶ����ռ��У�
//����ÿ���̶߳����Լ�������һ����.tls"���������Զ���ͬһ�������������ڲ�ͬ�߳��еĵ�ַ���ǲ�һ���ġ�
//ȫ�ֵı��������߳��������߳�˽�еģ�����Ҫ����
static _declspec(thread) ThreadCache* tls_threadcache = nullptr;