#pragma once

#include "Comment.h"


struct Memory{
	Memory(){
		leftsize = 1024 * 1;
		memory = (char *)malloc(leftsize);
		std::cout << (void *)memory << std::endl;
		head = memory;
	}

	char *memory = nullptr;//�ڴ�أ�����ڴ棬char*����һ�ֽڣ��������
	Memory *next = nullptr;//ָ����һ���ڴ��
	size_t leftsize = 0;//ʣ��ռ�
	char *head = nullptr;//�����ڴ����ʼ��ַ�������ͷ�
	//��������
	~Memory(){
		free(head);
	}
};

template<class T>
class ObjectPool{
private:
	//������ڴ�����������������ͷ�
	Memory *m = nullptr;
	void *freelist = nullptr;//��������ͷָ�룬�����û�ʹ����Ŀռ�

	//�����и���ڴ��С��ָ���С
	static  size_t GetSize(){
		if (sizeof(T) < sizeof(void*)){
			return sizeof(void*);
		}
		else{
			return sizeof(T);
		}
	}
	//ָ��Ҳ���Բ���Ҫ����
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
		//����ȥ����������û��
		//û��ȥ�ڴ������
		T* obj = nullptr;
		if (freelist){
			obj = (T*)freelist;
			//�������һ��λ��
			freelist = GetIndex(obj);
			return obj;
		}
		else{
			//����û���ڴ�أ���Ҫȥ������߿ռ䲻��
			if (m == nullptr || m->leftsize < sizeof(T)){
				//��������
				Memory *tmp = new Memory();
				tmp->next = m;
				m = tmp;
			}
			
			obj = (T*)m->memory;
			m->memory += GetSize();
			m->leftsize -= GetSize();
			//��λnew����obj�����ٿռ䣬���ҿ��Ե��ù��캯��
			new(obj)T;
			return obj;


		}


	}
	void Delete(T* obj){
		//��������
		obj->~T();
		//����free��������ռ䣬����free���ֿռ�
		//ֱ�����и���ڴ汣������һ������ĵ�ַ
		//ǿת��void**���ٽ����ã���ֹ��ͬƽ̨��ָ���ֽ�����ͬ
		//ͷ��
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
//	char *memory = nullptr;//�ڴ�أ�����ڴ�
//	size_t leftsize = 0;//ʣ��ռ�
//	void *freelist = nullptr;//��������ͷָ�룬�����û�ʹ����Ŀռ�
//
//	static  size_t GetSize(){
//		if (sizeof(T) < sizeof(void*)){
//			return sizeof(void*);
//		}
//		else{
//			return sizeof(T);
//		}
//	}
//	//ָ��Ҳ���Բ���Ҫ����
//	static void*& GetIndex(T*& obj){
//		return (*(void **)obj);
//	}
//public:
//	T* New(){
//		//����ȥ����������û��
//		//û��ȥ�ڴ������
//		T* obj = nullptr;
//		if (freelist){
//			obj = (T*)freelist;
//			//�������һ��λ��
//			freelist = GetIndex(obj);
//			return obj;
//		}
//		else{
//			//����û���ڴ�أ���Ҫȥ����
//			if (leftsize < sizeof(T)){
//				leftsize = 1024 * 1;
//				memory = (char *)malloc(leftsize);
//				//����ʧ�����쳣
//				if (memory == nullptr){
//					throw std::bad_alloc();
//				}
//			}
//			obj = (T*)memory;
//			memory += GetSize();
//			leftsize -= GetSize();
//			//��λnew����obj�����ٿռ䣬���ҿ��Ե��ù��캯��
//			new(obj)T;
//			return obj;
//
//
//		}
//
//
//	}
//	void Delete(T* obj){
//		//��������
//		obj->~T();
//		//����free��������ռ䣬����free���ֿռ�
//		
//		//ֱ�����и���ڴ汣������һ������ĵ�ַ
//		//ǿת��void**���ٽ����ã���ֹ��ͬƽ̨��ָ���ֽ�����ͬ
//		//ͷ��
//		GetIndex(obj) = freelist;
//		freelist = obj;
//	}
//
//
//
//
//};