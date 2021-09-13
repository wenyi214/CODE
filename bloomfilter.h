#pragma once
//��¡����������λͼʵ�ֵ�
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


//KToInt��K����ת����int,�����������һ������ת�ɶ�����Σ����λ����һ�����ݵ�״̬
template<class K, class KToInt1 = KeyToInt1<K>, class KToInt2 = KeyToInt2<K>, class KToInt3 = KeyToInt3<K>>
class bloomfilter{
public:
	bloomfilter(size_t num)
		:_bloom(num * 5)
		, _count(0)
	{}
		//����̫С�𲻵��������ã�Ҳ����̫�󣬱����о�������5���

	void insert(const K& num){
		//��ת������
		int index1 = KToInt1()(num) % _bloom.bitcount();//��������λ������ֹԽ��
		std::cout << index1 << std::endl;
		int index2 = KToInt2()(num) % _bloom.bitcount();
		std::cout << index2 << std::endl;

		int index3 = KToInt3()(num) % _bloom.bitcount();
		std::cout << index3 << std::endl;

		//����Ӧλ��1
		_bloom.set(index1);
		_bloom.set(index2);
		_bloom.set(index3);

	}
	//ֻҪ��һλΪ0���Ͳ�����
	bool IsBloomFilter(const K& num){
		int index1 = KToInt1()(num) % _bloom.bitcount();//��������λ������ֹԽ��
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
	size_t _count = 0;//���ݸ���
};

