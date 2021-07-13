#include<iostream>
#include<Windows.h>
using namespace std;


class Date{
public:
	//���캯��
	Date(int year = 0, int mouth = 1, int day = 1);
	//�������캯��
	Date(const Date& d);
	//���ÿ������������
	int GetMouthDay(int year, int mouth);

	void Print();

	//���������
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
	//ǰ��++
	Date& operator++();

	//����++
	Date operator++(int);
	//ǰ��--
	Date& operator--();

	//����--
	Date operator--(int);


	//���ڼ����ڣ���������
	int operator-(const Date& d)const;

private:
	int _year;
	int _mouth;
	int _day;
};
