#pragma once

#include "Comment.h"


struct Memory{
	Memory(){
		leftsize = 1024 * 1;
		memory = (char *)malloc(leftsize);
		std::cout << (void *)memory << std::endl;
		head = memory;
	}

	char *memory = nullptr;//内存池，大块内存，char*步长一字节，方便分配
	Memory *next = nullptr;//指向下一个内存池
	size_t leftsize = 0;//剩余空间
	char *head = nullptr;//保存内存块起始地址，方便释放
	//析构函数
	~Memory(){
		free(head);
	}
};

template<class T>
class ObjectPool{
private:
	//将大的内存块连接起来，方便释放
	Memory *m = nullptr;
	void *freelist = nullptr;//自由链表头指针，保存用户使用完的空间

	//可能切割的内存块小于指针大小
	static  size_t GetSize(){
		if (sizeof(T) < sizeof(void*)){
			return sizeof(void*);
		}
		else{
			return sizeof(T);
		}
	}
	//指针也可以不需要引用
	static void*& GetIndex(T*& obj){
		return (*(void **)obj);
	}

	void Destroy(){
		while (m){
			Memory *tmp = m;
			m = m->next;
			delete tmp;
			
		}
	}

public:
	T* New(){
		//首先去看链表中有没有
		//没有去内存池中切
		T* obj = nullptr;
		if (freelist){
			obj = (T*)freelist;
			//链表的下一个位置
			freelist = GetIndex(obj);
			return obj;
		}
		else{
			//可能没有内存池，需要去申请或者空间不够
			if (m == nullptr || m->leftsize < sizeof(T)){
				//连接起来
				Memory *tmp = new Memory();
				tmp->next = m;
				m = tmp;
			}
			
			obj = (T*)m->memory;
			m->memory += GetSize();
			m->leftsize -= GetSize();
			//定位new，再obj处开辟空间，并且可以调用构造函数
			new(obj)T;
			return obj;


		}


	}
	void Delete(T* obj){
		//调用析构
		obj->~T();
		//不能free，申请大块空间，不能free部分空间
		//直接用切割的内存保存子下一个链表的地址
		//强转成void**，再解引用，防止不同平台下指针字节数不同
		//头插
		GetIndex(obj) = freelist;
		freelist = obj;
	}

	~ObjectPool(){
		Destroy();
	}


};

//template<class T>
//class ObjectPool{
//private:
//	char *memory = nullptr;//内存池，大块内存
//	size_t leftsize = 0;//剩余空间
//	void *freelist = nullptr;//自由链表头指针，保存用户使用完的空间
//
//	static  size_t GetSize(){
//		if (sizeof(T) < sizeof(void*)){
//			return sizeof(void*);
//		}
//		else{
//			return sizeof(T);
//		}
//	}
//	//指针也可以不需要引用
//	static void*& GetIndex(T*& obj){
//		return (*(void **)obj);
//	}
//public:
//	T* New(){
//		//首先去看链表中有没有
//		//没有去内存池中切
//		T* obj = nullptr;
//		if (freelist){
//			obj = (T*)freelist;
//			//链表的下一个位置
//			freelist = GetIndex(obj);
//			return obj;
//		}
//		else{
//			//可能没有内存池，需要去申请
//			if (leftsize < sizeof(T)){
//				leftsize = 1024 * 1;
//				memory = (char *)malloc(leftsize);
//				//申请失败抛异常
//				if (memory == nullptr){
//					throw std::bad_alloc();
//				}
//			}
//			obj = (T*)memory;
//			memory += GetSize();
//			leftsize -= GetSize();
//			//定位new，再obj处开辟空间，并且可以调用构造函数
//			new(obj)T;
//			return obj;
//
//
//		}
//
//
//	}
//	void Delete(T* obj){
//		//调用析构
//		obj->~T();
//		//不能free，申请大块空间，不能free部分空间
//		
//		//直接用切割的内存保存子下一个链表的地址
//		//强转成void**，再解引用，防止不同平台下指针字节数不同
//		//头插
//		GetIndex(obj) = freelist;
//		freelist = obj;
//	}
//
//
//
//
//};