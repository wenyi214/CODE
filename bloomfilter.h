#pragma once
//布隆过滤器是用位图实现的
#include"bitset.h"
#include<string>

template<class T>
struct KeyToInt1{
	T& operator()(const T& k){
		return k;
	}
};

template<>
struct KeyToInt1<std::string>
{
	size_t operator()(const std::string& s){
		if (s.size() == 0){
			return 0;
		}
		
		size_t hash = 0;
		for (size_t i = 0; i < s.size(); i++){
			hash *= 131;
			hash += s[i];
		}
		return hash;
	}
};

template<class T>
struct KeyToInt2{
	T& operator()(const T& k){
		return k;
	}
};

template<>
struct KeyToInt2<std::string>
{
	size_t operator()(const std::string& s){
		if (s.size() == 0){
			return 0;
		}
		
		size_t hash = 0;
		for (size_t i = 0; i < s.size(); i++){
			hash *= 65599;
			hash += s[i] ;
		}
		return hash;
	}
};

template<class T>
struct KeyToInt3{
	T& operator()(const T& k){
		return k;
	}
};

template<>
struct KeyToInt3<std::string>
{
	size_t operator()(const std::string& s){
		if (s.size() == 0){
			return 0;
		}
		int magic = 63689;
		size_t hash = 0;
		for (size_t i = 0; i < s.size(); i++){
			hash *= magic;
			hash += s[i];
			magic *= 378551;
		}
		return hash;
	}
};


//KToInt是K类型转化成int,多个函数，将一个数据转成多个整形，多个位保存一个数据的状态
template<class K, class KToInt1 = KeyToInt1<K>, class KToInt2 = KeyToInt2<K>, class KToInt3 = KeyToInt3<K>>
class bloomfilter{
public:
	bloomfilter(size_t num)
		:_bloom(num * 5)
		, _count(0)
	{}
		//长度太小起不到过滤作用，也不能太大，别人研究出来乘5最好

	void insert(const K& num){
		//先转成整形
		int index1 = KToInt1()(num) % _bloom.bitcount();//余上所有位数，防止越界
		std::cout << index1 << std::endl;
		int index2 = KToInt2()(num) % _bloom.bitcount();
		std::cout << index2 << std::endl;

		int index3 = KToInt3()(num) % _bloom.bitcount();
		std::cout << index3 << std::endl;

		//将对应位置1
		_bloom.set(index1);
		_bloom.set(index2);
		_bloom.set(index3);

	}
	//只要有一位为0，就不存在
	bool IsBloomFilter(const K& num){
		int index1 = KToInt1()(num) % _bloom.bitcount();//余上所有位数，防止越界
		if (!_bloom.test(index1)){
			return false;
		}
		int index2 = KToInt2()(num) % _bloom.bitcount();
		if (!_bloom.test(index2)){
			return false;
		}
		int index3 = KToInt3()(num) % _bloom.bitcount();
		if (!_bloom.test(index3)){
			return false;
		}
		return true;
	}
private:
	bitset _bloom;
	size_t _count = 0;//数据个数
};

