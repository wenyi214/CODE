#pragma once
#include<iostream>
#include<deque>
#include<vector>
#include<list>
using namespace std;

namespace my{
	//仿函数
	template<class T>
	struct less{
		bool operator()(const T& left,const T& right){
			return left < right;
		}
	};
	template<class T>
	struct greater{
		bool operator()(const T& left, const T& right){
			return left>right;
		}
	};


	template<class T,class Container = vector<T>,class Compare=less<T>>
	class priority_queue{
	public:
		priority_queue(){};
		template<class inputiterator>
		priority_queue(inputiterator first, inputiterator last){
			while (first != last){
				push(*first);
				first++;
			}
		}
		bool empty(){
			return _con.empty();
		}
		size_t size(){
			return _con.size();
		}
		T& top(){
			return _con.front();
		}
		const T& top()const{
			return _con.front();
		}
		void push(const T& val){
			_con.push_back(val);
			Adjustup(_con.size() - 1);
		}
		void pop(){
			swap(_con[0], _con[_con.size() - 1]);
			//少一个元素是size减减，交换后，直接删除最后一个元素
			_con.pop_back();
			Adjustdown(0);
		}
	private:
		void Adjustup(int child){
			int parent = (child - 1) / 2;
			Compare com;
			while (child){
				if (com(_con[parent],_con[child])){
					swap(_con[child], _con[parent]);
					child = parent;
					parent = (child - 1) / 2;
				}
				else{
					break;
				}
			}
		}

		void Adjustdown(int parent){
			int child = parent * 2 + 1;
			//定义一个对象
			Compare com;
			//不能减1，如果没有元素size()为0，类型size_t,减1就是最大的数(正数)，会进入循环
			while ((size_t)child < _con. size() ){
				
				if (child + 1 < _con.size() && com(_con[child], _con[child + 1])){
					child++;
				}
				if (com(_con[parent], _con[child])){
					swap(_con[child], _con[parent]);
					parent = child;
					child = parent * 2 + 1;
				}
				else{
					break;
				}
			}
		}

		Container _con;
	};

	void test_priority_queue(){
		int array[] = { 8, 4, 6, 2, 10 };
		priority_queue<int,vector<int>,greater<int>> pq(array, array + sizeof(array) / sizeof(int));
		cout << pq.size() << endl;

		while (!pq.empty()){
			cout << pq.top() << " ";
			pq.pop();
		}
		cout << endl;

	}
}