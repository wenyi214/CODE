#pragma once

#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<fstream>
using namespace std;

#define NUM 1000
//联系人信息
struct PersonMsg{

	string _name;
	string _age;
	string _sex;
	string _telephone;
	string _address;
};
//比较仿函数
struct Compare{
	bool operator()(PersonMsg* p1,PersonMsg* p2){
		return p1->_name < p2->_name;
	}
};



class File{
private:
	
public:
	File(const char *filename = "addressbook.text")
		
	{}
	

	bool Read(ifstream* ifs, PersonMsg* pmsg){

		if ((*ifs >> pmsg->_name) && (*ifs >> pmsg->_sex) && (*ifs >> pmsg->_age) && (*ifs >> pmsg->_telephone) && (*ifs >> pmsg->_address))
			return false;

		return true;
	}


	void Write(ofstream* ofs, const PersonMsg* pmsg){
		
		*ofs << pmsg->_name << endl;
		*ofs << pmsg->_sex << endl;
		*ofs << pmsg->_age << endl;
		*ofs << pmsg->_telephone << endl;
		*ofs << pmsg->_address << endl;
		
	}


	~File(){

	}
};




class AddressBook{
private:
	vector<PersonMsg*> _book;
	size_t _size;
	size_t _capacity;
	//File _fl;

private:
	void Print(int i){
		cout << _book[i]->_name << "     " << _book[i]->_sex << "     " << _book[i]->_age << "     " << _book[i]->_telephone << "     " << _book[i]->_address << endl;
	}

	size_t Find(const string& name){
		size_t i = 0;
		for (; i < _size; i++){
			if (_book[i]->_name == name){//C语言用strcmp
				break;
			}
		}
		if (i >= _size){
			return -1;
		}
		return i;
		
	}

	void ModifyMenu(){
		cout << "请输入修改信息对应的序号" << endl;
		cout << "-------------" << endl;
		cout << "--  1.名字 --" << endl;
		cout << "--  2.性别 --" << endl;
		cout << "--  3.年龄 --" << endl;
		cout << "--  4.电话 --" << endl;
		cout << "--  5.地址 --" << endl;
		cout << "-------------" << endl;

	}

	void Write(ofstream* ofs, const PersonMsg* pmsg){

		*ofs << pmsg->_name << endl;
		*ofs << pmsg->_sex << endl;
		*ofs << pmsg->_age << endl;
		*ofs << pmsg->_telephone << endl;
		*ofs << pmsg->_address << endl;

	}
	bool Read(ifstream* ifs, PersonMsg* pmsg){

		if ((*ifs >> pmsg->_name) && (*ifs >> pmsg->_sex) && (*ifs >> pmsg->_age) && (*ifs >> pmsg->_telephone) && (*ifs >> pmsg->_address))
			return false;

		return true;
	}


public:
	AddressBook()
		:_size(0)
		, _capacity(NUM)
	{
		_book.resize(NUM);
	}

	void AddressDook2File(ifstream *r){
		
		PersonMsg *tmp = new PersonMsg;
		while (!Read(r,tmp)){
			
			_book[_size++] = tmp;
			if (_size == _capacity){
				_book.resize(2 * _capacity);
				_capacity *= 2;
			}
			tmp = new PersonMsg;
		}
		//最后会多一个
		delete tmp;
	}
	void File2AddressDook(ofstream *w){

		for (int i = 0; i < _size; i++){
			Write(w,_book[i]);
		}
	}

	//添加联系人信息
	void AddPerson(){
		//扩容
		if (_size == _capacity){
			_book.resize(2 * _capacity);
			_capacity *= 2;

			//C语言，做深拷贝
			//重新申请空间，将指针都拷贝过去，释放旧空间，让指针指向新开辟的空间。
		}
		
		PersonMsg *person = new PersonMsg;
		cout << "请输入名字# " ;
		cin >> person->_name;
		cout << "请输入性别# " ;
		cin >> person->_sex;
		cout << "请输入年龄# " ;
		cin >> person->_age;
		cout << "请输入电话# " ;
		cin >> person->_telephone;
		cout << "请输入地址# " ;
		cin >> person->_address;

		_book[_size++] = person;
		cout << "添加成功" << endl;
		

	}
	//删除联系人信息
	void DeletePerson(const string& name){
		if (_size == 0){
			cout << "通讯里为空，删除失败！！！" << endl;
		}
		else{
			
			size_t pos = Find(name);
			if (pos == -1){
				cout << "查无此人！！！" << endl;
			}
			else{
				for (int i = pos; i < _size - 1; i++){
					_book[i] = _book[i + 1];
				}
				_size--;
				cout << "删除成功" << endl;

			}
		}
	}
	
	//修改联系人信息
	void ModifyMsg(const string& name){
		size_t pos = Find(name);
		if (pos == -1){
			cout << "查无此人！！！" << endl;
		}
		else{
			ModifyMenu();
			int opt = 0;
			cin >> opt;
			switch (opt)
			{
			case 1:
				cout << "请输入名字# ";
				cin >> _book[pos]->_name;
				break;
			case 2:
				cout << "请输入性别# ";
				cin >> _book[pos]->_sex;
				break;
			case 3:
				cout << "请输入年龄# ";
				cin >> _book[pos]->_age;
				break;
			case 4:
				cout << "请输入电话# ";
				cin >> _book[pos]->_telephone;
				break;
			case 5:
				cout << "请输入地址# ";
				cin >> _book[pos]->_address;
				break;
			default:
				cout << "输入错误" << endl;
				break;
			}
		}
	}

	//打印信息

	void PrintBook(){
		char *pace = "   ";
		cout << "Name" << pace << "Sex" << pace << "Age" << pace << "telephone" << pace << "Address" << endl;
		for (int i = 0; i < _size; i++){
			Print(i);
		}
	}

	//查找联系人信息
	void FindPerson(const string& name){
		size_t pos = Find(name);
		if (pos == -1){
			cout << "查无此人！！！" << endl;
		}
		else{
			char *pace = "   ";
			cout << "Name" << pace << "Sex" << pace << "Age" << pace << "telephone" << pace << "Address" << endl;
			Print(pos);
		}
	}
	//排序
	void SortBook(){
		//Compare仿函数，不是到end()，而是到size处
		sort(_book.begin(), _book.begin()+_size, Compare());
		cout << "升序排序成功" << endl;
	}

	//清空所有联系人
	void Clear(){
		_size = 0;
		cout << "清除成功" << endl;
	}




	~AddressBook()
	{
		for (size_t i = 0; i < _size; i++){
			delete _book[i];
		}
	}


};