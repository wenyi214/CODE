#include<iostream>
#include<assert.h>
#include<string>
#include<Windows.h>
#pragma warning(disable:4996)
using namespace std;

namespace my{
	class string{
		friend istream& operator>>(istream& in, string& s);
	public:
		typedef char* iterator;
		iterator begin(){
			return _str;
		}
		iterator end(){
			return _str + _size;
		}
		//Ҫ�ڶ��Ͽ��ٿռ佫�ַ���������ȥ������ֱ���ڳ�ʼ���б����ʼ�������򲻿��޸�
		//���ַ���ҲҪ����һ���ռ䣬��\0
		//�������캯��
		string(const char *str="")
		:_str(new char[strlen(str)+1])
		{
			strcpy(_str, str);
			_size = strlen(str);
			_capacity = _size;
		}
		//�������캯��
		string(const string& s){
			_str = new char[strlen(s._str) + 1];
			strcpy(_str, s._str);
			_size = s._size;
			_capacity = s._capacity;
			
		}
		//��������
		~string(){
			delete[] _str;
			_str = nullptr;
		}
		//��ֵ������
		string& operator=(const string& s){
			if (this != &s){
				char *newstr = new char[strlen(s._str) + 1];
				strcpy(newstr, s._str);
				delete[] _str;
				_str = newstr;
				_size = s._size;
				_capacity = s._capacity;
				
			}
			return *this;
		}

		char& operator[](size_t pos){
			return _str[pos];
		}
		const char& operator[](size_t pos)const{
			return _str[pos];
		}

		size_t size()const{
			return _size;
		}
		size_t capacity()const{
			return _capacity;
		}
		const char *c_str(){
			return _str;
		}

		void push_back(char c){
			if (_size == _capacity){
				//ϸ�� �ж������Ƿ����0
				size_t newcapacity = _capacity == 0 ? 2 : 2 * _capacity;
				//��1  Ԥ��\0λ��
				char *newstr = new char[newcapacity+1];
				strcpy(newstr, _str);
				delete[] _str;
				_str = newstr;
				_capacity = newcapacity;
			}
			_str[_size++] = c;
			//Ҫ��ĩβ����\0
			_str[_size] = '\0';
			
		}

		void append(const char *s){
			size_t len = strlen(s) + _size;
			if (len > _capacity){
				char *newstr = new char[len + 1];
				strcpy(newstr, _str);
				delete[] _str;
				_str = newstr;
				_capacity = len;
			}
			strcpy(_str + _size, s);
			_size = len;
			
		}
		//����push_back
		string& operator+=(char c){
			push_back(c);
			return *this;
		}
		//����append
		string& operator+=(const char *s){
			append(s);
			return *this;
		}
		//����Ҫ����ֵ���ں�������this�ı���
		void resize(size_t len, char c = '\0'){
			if (len > _size){
				if (len > _capacity){
					char *temp = new char[len + 1];
					strcpy(temp, _str);
					delete[] _str;
					_str = temp;
					_capacity = len;
				}
				//��memset����
				memset(_str + _size, c, len - _size);

			}
			_size = len;
			_str[_size] = '\0';
		}
		void reserve(size_t newcapacity){
			//ֻ�д�������ʱ������
			if (newcapacity > _capacity){
				char *newstr = new char[newcapacity + 1];
				strcpy(newstr, _str);
				delete[] _str;
				_str = newstr;
				_capacity = newcapacity;

			}
		}
		string& insert(size_t pos, char c){
			assert(pos < _size);
				if (_size==_capacity){
					int newcapacity = _capacity == 0 ? 2 : 2*_capacity;
					reserve(newcapacity);
				}
				size_t end = _size;
				while (end >= pos){
					_str[end + 1] = _str[end];
					end--;
				}
				_str[pos] = c;
				_size++;

				return *this;

		}
		string& insert(size_t pos, const char *s){
			assert(pos < _size);
			int len = strlen(s);
			if (len + _size>_capacity){
				reserve(len + _size);
			}
			size_t end = _size;
			while (end >= pos){
				_str[end + len] = _str[end];
				end--;
			}
			strncpy(_str + pos, s, len);	
			_size += len;
			return *this;

		}
		//���������ɾ�����ȼ���λ��С��_size��ɾ�����ȼ���λ�ô��ڵ���_size��
		string& erase(size_t pos, size_t len = npos){
			assert(pos < _size);
			//�ü��ȽϺã���ֹԽ�磬�ӵĻ�npos���ֵ������һֵ����Խ��
			if (len >= _size - pos){
				_str[pos] = '\0';
				_size = pos;
			}
			else{
				size_t i = pos + len;
				while (i <= _size){
					_str[pos++] = _str[i];
					i++;
				}
				_size -= len;
			}
			return *this;

		}

		size_t find(char c){
			for (size_t i = 0; i < size(); i++){
				if (_str[i] == c){
					return i;
				}
			}
			return npos;
		}

		size_t find(char *s){
			//ʹ��C�����ַ�������
			char *ret = strstr(_str, s);
			if (ret == NULL){
				return npos;
			}
			return ret - _str;

		}

		bool operator<(const string& s)const{
			int key = strcmp(_str, s._str);
			return key < 0;

		}
		bool operator==(const string& s)const{
			int key = strcmp(_str, s._str);
			return key == 0;
		}
		bool operator<=(const string& s)const{
			return *this<s || *this == s;
		}
		bool operator>(const string& s)const{
			return !(*this <= s);

		}
		bool operator>=(const string& s)const{
			return !(*this < s);

		}
		bool operator!=(const string& s){
			return !(*this == s);
		}




	private:
		char *_str;
		size_t _size;
		size_t _capacity;

		static size_t npos;
	};

	size_t string::npos = -1;

	ostream& operator<<(ostream& out, const string& s){
		for (size_t i = 0; i < s.size(); i++){
			out << s[i];
		}
		return out;
	}

	istream& operator>>(istream& in, string& s){

		while (1){
			//cinʱistream��Ķ���getʱistream���Ա����
			char ch = in.get();
			if (ch == ' ' || ch == '\n'){
				break;
			}
			s += ch;
		}
		return in;

	}

	
}

