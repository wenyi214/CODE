#include"dateclass.h"

Date::Date(int year, int mouth, int day) {
	_year = year;
	_mouth = mouth;
	_day = day;
}
Date::Date(const Date& d){
	_year = d._year;
	_mouth = d._mouth;
	_day = d._day;
}
int Date::GetMouthDay(int year, int mouth){
	int ass[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (mouth == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))){
		ass[mouth] = 29;
	}

	return ass[mouth];
}

void Date::Print(){
	if (_year >= 0 && _mouth > 0 && _mouth <= 12 && _day > 0 && _day <= GetMouthDay(_year, _mouth)){
		cout << _year << "-" << _mouth << "-" << _day << endl;
	}
	else{
		cout << "error!" << endl;
	}
}

//重载运算符
//d1=d2
Date& Date::operator=(const Date& d){
	_year = d._year;
	_mouth = d._mouth;
	_day = d._day;

	return *this;
}


//d1>d2
bool Date::operator>(const Date& d)const{
	if (_year > d._year){
		return true;
	}
	else if (_year == d._year&&_mouth > d._mouth){
		return true;
	}
	else if (_year == d._year&&_mouth == d._mouth&&_day > d._day){
		return true;
	}
	else{
		return false;
	}
}

bool Date::operator==(const Date& d)const{
	return _year == d._year&&_mouth == d._mouth&&_day == d._day;
}
bool Date::operator>=(const Date& d)const{
	return ((*this) > d) || ((*this) == d);
}
bool Date::operator!=(const Date& d)const{
	return !(*this == d);
}

bool Date::operator<(const Date& d)const{
	return !((*this)>d || (*this) == d);
}
bool Date::operator<=(const Date& d)const{
	return !((*this) > d);
}


//d1+x   
Date Date:: operator+(int day)const{
	//d1没变
	Date ret = *this;//*this等价于d1

	ret._day += day;
	while (ret._day > ret.GetMouthDay(ret._year, ret._mouth)){
		ret._day -= ret.GetMouthDay(ret._year, ret._mouth);
		ret._mouth++;

		if (ret._mouth > 12){
			ret._mouth -= 12;
			ret._year++;
		}

	}
	return ret;

}
//d1+=x
Date& Date:: operator+=(int day){


	_day += day;
	while (_day > GetMouthDay(_year, _mouth)){
		_day -= GetMouthDay(_year, _mouth);
		_mouth++;

		if (_mouth > 12){
			_mouth -= 12;
			_year++;
		}

	}
	return *this;
}
//d1-x
Date Date::operator-(int day)const{
	Date ret = *this;

	ret._day -= day;

	while (ret._day <= 0){
		ret._day += ret.GetMouthDay(ret._year, ret._mouth);
		ret._mouth--;

		if (ret._mouth < 1){
			ret._year--;
			ret._mouth = 12;
		}
	}
	return ret;

}
//d1-=x
Date& Date::operator-=(int day){

	_day -= day;

	while (_day <= 0){
		_day += GetMouthDay(_year, _mouth);
		_mouth--;

		if (_mouth < 1){
			_year--;
			_mouth = 12;
		}
	}
	return *this;

}
//前置++
Date& Date::operator++(){
	(*this)._day += 1;
	return *this;
}

//后置++
Date Date::operator++(int){
	Date ret(*this);
	(*this)._day += 1;
	return ret;
}
//前置--
Date& Date::operator--(){
	(*this)._day -= 1;
	return *this;
}

//后置--
Date Date::operator--(int){
	Date ret(*this);
	(*this)._day -= 1;
	return ret;
}


//日期减日期，返回天数
int Date::operator-(const Date& d)const{
	int flag = 1;
	Date Min(d);
	Date Max(*this);

	//找到大的日期，看相差多少天
	if ((*this) < d){
		Min = *this;
		Max = d;
		flag = -1;
	}

	int c = 1;
	int day = 0;
	while (Max != Min){
		Min += c;
		day++;
	}
	return day*flag;

}


