#include<iostream>
#include<assert.h>
#include<Windows.h>
#pragma warning(disable:4996)
using namespace std;


class String{
public:
	//���캯��
	String(const char *str = "")
		:_str(new char[strlen(str)+1])
	{
		strcpy(_str, str);
	}
	//�������캯��
	String(const String& s)
		//Ҫ��ʼ��Ϊ�գ���Ȼ�����ֵ(Ұָ��)��������tmp���������������������
		:_str(nullptr)
	{
		String tmp(s._str);
		swap(_str, tmp._str);
	}
	//��������
	~String(){
		delete[] _str;
		_str = nullptr;
	}
	//��ֵ���������غ���
	String& operator=(const String& s){
		String tmp(s._str);
		swap(_str, tmp._str);
		return *this;
	}
	//��ֵ�����ÿ������캯�����൱�ڴ�����һ����ʱ����
	//String& operator=(String s){
	//	swap(_str, s._str);
	//	return *this;
	//}


	size_t size(){
		return strlen(_str);
	}

	char& operator[](size_t pos){
		assert(pos < size());
		return _str[pos];
	}

private:
	char *_str;

};

