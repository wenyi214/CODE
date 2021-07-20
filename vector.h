#pragma once

#include<iostream>
#include<Windows.h>
#include<assert.h>
using namespace std;

template<class T>
class vector{
public:
	typedef T * iterator;
	typedef const T* const_iterator;
	//迭代器
	iterator begin(){
		return _start;
	}
	iterator end(){
		return _finish;
	}
	const_iterator cbegin(){
		return _start;
	}
	const_iterator cend(){
		return _finish;
	}
	
	vector()
		:_start(nullptr)
		, _finish(nullptr)
		, _endofstorage(nullptr)
	{}
	vector(size_t n, const T& val = T())
		:_start(nullptr)
		, _finish(nullptr)
		, _endofstorage(nullptr)
	{
		reserve(n);
		while (_finish != (_start + n)){
			*_finish = val;
			_finish++;
		}

	}

	//不好调用Swap函数，不能调用拷贝构造函数，只能调构造函数
	vector(const vector<T>& v)
		:_start(nullptr)
		, _finish(nullptr)
		, _endofstorage(nullptr)
	{
		reserve(v.capacity());
		for (size_t i = 0; i < v.size(); i++){
			*_finish = v[i];
			_finish++;
		}

	}

	vector<int>& operator=(vector<int> v){
		Swap(v);
		return *this;

	}
	~vector(){
		delete[] _start;
		_start = _finish = _endofstorage = nullptr;
	}

	T& operator[](size_t pos){
		assert(pos < size());
		return _start[pos];
	}
	const T& operator[](size_t pos)const{
		assert(pos < size());
		return _start[pos];
	}

	void push_back(const T& val){
		//if (_finish == _endofstorage){
		//	size_t newcapacity = capacity() == 0 ? 2 : 2 * capacity();
		//	reserve(newcapacity);
		//}
		//*_finish = val;
		//_finish++;
		insert(_finish, val);

	}


	iterator insert(iterator pos, const T& val){
		assert(pos <= _finish);
		if (_finish == _endofstorage){
			//扩容后pos位置迭代器失效了，记录位置，重新更新pos位置
			size_t sz = pos - _start;
			size_t newcapacity = capacity() == 0 ? 2 : 2 * capacity();
			reserve(newcapacity);
			//更新pos位置
			pos = _start + sz;
		}
		
		iterator end = _finish - 1;
		while (end >= pos){
			*(end + 1) = *end;
			end--;
		}
		
		*pos = val;
		_finish++;
		return pos;
	}

	void pop_back(){
		assert(_finish != _start);
		//_finish--;

		erase(_finish - 1);
	}

	iterator erase(iterator pos){
		assert(pos < _finish);

		iterator tmp = pos;
		while (tmp < _finish){
			*tmp = *(tmp + 1);
			tmp++;
		}
		_finish--;
		return pos;
	}


	size_t size()const{
		return _finish - _start;
	}

	size_t capacity()const{
		return _endofstorage - _start;
	}

	vector<T>& reserve(size_t n){
		if (n > capacity()){
			size_t oldsize = size();
			T *tmp = new T[n];
			if (_start){
				//使用赋值，调用T的赋值操作符重载函数
				for (size_t i = 0; i < size(); i++){
					tmp[i] = _start[i];
				}
				delete[] _start;
			}
			_start = tmp;
			_finish = _start + oldsize;
			_endofstorage = _start + n;
	
		}
		return *this;
	}

	void resize(size_t n, const T& val = T()){
		if (n > size()){
			if (n > capacity()){
				size_t newcapacity = capacity() == 0 ? 2 : 2 * capacity();
				reserve(newcapacity);
			}
			while (_finish != (_start + n)){
				*_finish = val;
				_finish++;
			}

		}
		else{
			_finish = _start + n;
		}
		
	}

	void Swap(vector<int>& v){
		swap(_start, v._start);
		swap(_finish, v._finish);
		swap(_endofstorage, v._endofstorage);

	}


private:
	iterator _start;
	iterator _finish;
	iterator _endofstorage;
};