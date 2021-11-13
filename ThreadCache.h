#pragma once

#include "Comment.h"

//结构为一个哈希桶，桶里连接的小块内存
class ThreadCache{
private:
	//结构体数组，结构体里边是一个保存链表头指针
	FreeList freelist[MAX_BYTES];//按对齐数映射
public:
	ThreadCache()
	{}
	//链表太长，需要合并到centralcache的span中
	void ListTooLong(FreeList list, size_t size);

	//声明和定义分离
	//申请内存
	void *Allocate(size_t size);
	//返还内存
	void Deallocate(void *ptr, size_t size);
	//向centralcache申请内存
	void* FetchFromCentral(size_t size);

	~ThreadCache()
	{}

};
//怎么做到Thread Cache线程私有？TLS
//TLS thread local storage线程本地存储
//一旦一个全局变量被定义程TLS类型的，那么每个线程都会拥有这个变量的一个副本，任何线程对该变量的修改都不会影响其他线程中该变量的副本。
// 当使用_declspec(thread)定义一个线程私有变量时，编译器会把这些变量放到PE文件的“.tls"段中。
//当系统启动一个新的线程时，它会从进程的堆中分配一块足够大小的空间，然后把”.tls"段中的内容复制到这块空间中，
//于是每个线程都有自己独立的一个“.tls"副本。所以对于同一个变量，它们在不同线程中的地址都是不一样的。
//全局的变量，多线程下又是线程私有的，不需要加锁
static _declspec(thread) ThreadCache* tls_threadcache = nullptr;