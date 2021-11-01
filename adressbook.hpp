#pragma once

#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<fstream>
using namespace std;

#define NUM 1000
//��ϵ����Ϣ
struct PersonMsg{

	string _name;
	string _age;
	string _sex;
	string _telephone;
	string _address;
};
//�ȽϷº���
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
			if (_book[i]->_name == name){//C������strcmp
				break;
			}
		}
		if (i >= _size){
			return -1;
		}
		return i;
		
	}

	void ModifyMenu(){
		cout << "�������޸���Ϣ��Ӧ�����" << endl;
		cout << "-------------" << endl;
		cout << "--  1.���� --" << endl;
		cout << "--  2.�Ա� --" << endl;
		cout << "--  3.���� --" << endl;
		cout << "--  4.�绰 --" << endl;
		cout << "--  5.��ַ --" << endl;
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
		//�����һ��
		delete tmp;
	}
	void File2AddressDook(ofstream *w){

		for (int i = 0; i < _size; i++){
			Write(w,_book[i]);
		}
	}

	//�����ϵ����Ϣ
	void AddPerson(){
		//����
		if (_size == _capacity){
			_book.resize(2 * _capacity);
			_capacity *= 2;

			//C���ԣ������
			//��������ռ䣬��ָ�붼������ȥ���ͷžɿռ䣬��ָ��ָ���¿��ٵĿռ䡣
		}
		
		PersonMsg *person = new PersonMsg;
		cout << "����������# " ;
		cin >> person->_name;
		cout << "�������Ա�# " ;
		cin >> person->_sex;
		cout << "����������# " ;
		cin >> person->_age;
		cout << "������绰# " ;
		cin >> person->_telephone;
		cout << "�������ַ# " ;
		cin >> person->_address;

		_book[_size++] = person;
		cout << "��ӳɹ�" << endl;
		

	}
	//ɾ����ϵ����Ϣ
	void DeletePerson(const string& name){
		if (_size == 0){
			cout << "ͨѶ��Ϊ�գ�ɾ��ʧ�ܣ�����" << endl;
		}
		else{
			
			size_t pos = Find(name);
			if (pos == -1){
				cout << "���޴��ˣ�����" << endl;
			}
			else{
				for (int i = pos; i < _size - 1; i++){
					_book[i] = _book[i + 1];
				}
				_size--;
				cout << "ɾ���ɹ�" << endl;

			}
		}
	}
	
	//�޸���ϵ����Ϣ
	void ModifyMsg(const string& name){
		size_t pos = Find(name);
		if (pos == -1){
			cout << "���޴��ˣ�����" << endl;
		}
		else{
			ModifyMenu();
			int opt = 0;
			cin >> opt;
			switch (opt)
			{
			case 1:
				cout << "����������# ";
				cin >> _book[pos]->_name;
				break;
			case 2:
				cout << "�������Ա�# ";
				cin >> _book[pos]->_sex;
				break;
			case 3:
				cout << "����������# ";
				cin >> _book[pos]->_age;
				break;
			case 4:
				cout << "������绰# ";
				cin >> _book[pos]->_telephone;
				break;
			case 5:
				cout << "�������ַ# ";
				cin >> _book[pos]->_address;
				break;
			default:
				cout << "�������" << endl;
				break;
			}
		}
	}

	//��ӡ��Ϣ

	void PrintBook(){
		char *pace = "   ";
		cout << "Name" << pace << "Sex" << pace << "Age" << pace << "telephone" << pace << "Address" << endl;
		for (int i = 0; i < _size; i++){
			Print(i);
		}
	}

	//������ϵ����Ϣ
	void FindPerson(const string& name){
		size_t pos = Find(name);
		if (pos == -1){
			cout << "���޴��ˣ�����" << endl;
		}
		else{
			char *pace = "   ";
			cout << "Name" << pace << "Sex" << pace << "Age" << pace << "telephone" << pace << "Address" << endl;
			Print(pos);
		}
	}
	//����
	void SortBook(){
		//Compare�º��������ǵ�end()�����ǵ�size��
		sort(_book.begin(), _book.begin()+_size, Compare());
		cout << "��������ɹ�" << endl;
	}

	//���������ϵ��
	void Clear(){
		_size = 0;
		cout << "����ɹ�" << endl;
	}




	~AddressBook()
	{
		for (size_t i = 0; i < _size; i++){
			delete _book[i];
		}
	}


};