#include<iostream>
#include<Windows.h>
using namespace std;


class Date{
public:
	//构造函数
	Date(int year = 0, int mouth = 1, int day = 1);
	//拷贝构造函数
	Date(const Date& d);
	//获得每个月最多多少天
	int GetMouthDay(int year, int mouth);

	void Print();

	//重载运算符
	//d1=d2
	Date& operator=(const Date& d);


	//d1>d2
	bool operator>(const Date& d)const;

	bool operator==(const Date& d)const;
	bool operator>=(const Date& d)const;
	bool operator!=(const Date& d)const;

	bool operator<(const Date& d)const;
	bool operator<=(const Date& d)const;


	//d1+x   
	Date operator+(int day)const;
	//d1+=x
	Date& operator+=(int day);
	//d1-x
	Date operator-(int day)const;
	
	//d1-=x
	Date& operator-=(int day);
	//前置++
	Date& operator++();

	//后置++
	Date operator++(int);
	//前置--
	Date& operator--();

	//后置--
	Date operator--(int);


	//日期减日期，返回天数
	int operator-(const Date& d)const;

private:
	int _year;
	int _mouth;
	int _day;
};
