#include<iostream>
#include<assert.h>
#include<Windows.h>
#pragma warning(disable:4996)
using namespace std;


class String{
public:
	//构造函数
	String(const char *str = "")
		:_str(new char[strlen(str)+1])
	{
		strcpy(_str, str);
	}
	//拷贝构造函数
	String(const String& s)
		//要初始化为空，不然是随机值(野指针)，交换到tmp后出作用域析构，程序奔溃
		:_str(nullptr)
	{
		String tmp(s._str);
		swap(_str, tmp._str);
	}
	//析构函数
	~String(){
		delete[] _str;
		_str = nullptr;
	}
	//赋值操作符重载函数
	String& operator=(const String& s){
		String tmp(s._str);
		swap(_str, tmp._str);
		return *this;
	}
	//传值，调用拷贝构造函数，相当于创建了一个临时变量
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

