#include<iostream>
#include<deque>
#include<vector>
#include<list>
using namespace std;

//namespace my{
//	template<class T,class Container=deque<T>>
//
//	class stack{
//	public:
//		bool empty(){
//			return _con.empty();
//		}
//		size_t size(){
//			return _con.size();
//		}
//		T& top(){
//			return _con.back();
//		}
//		const T& top()const{
//			return back();
//		}
//		void push(const T& val){
//			_con.push_back(val);
//
//		}
//		void pop(){
//			_con.pop_back();
//		}
//	private:
//		Container _con;
//
//	};
//
//	void test_stack(){
//		stack<int> s;
//
//		s.push(1);
//		s.push(2);
//		s.push(3);
//		s.push(4);
//
//		while (!s.empty())
//		{
//			cout << s.top() << " ";
//			s.pop();
//		}
//		cout << endl;
//	}
//
//	void test_stack1(){
//		stack<int,list<int>> s;
//
//		s.push(1);
//		s.push(2);
//		s.push(3);
//		s.push(4);
//
//		while (!s.empty())
//		{
//			cout << s.top() << " ";
//			s.pop();
//		}
//		cout << endl;
//	}
//}

namespace my{
	template<class T,class Container=deque<T>>
	class queue{
	public:
		queue(){};
		size_t size()const{
			return _con.size();
		}
		bool empty()const{
			return _con.empty();
		}
		T& back(){
			return _con.back();
		}
		T& front(){
			return _con.front();
		}
		const T& back()const{
			return _con.back();
		}
		const T& front()const{
			return _con.front();
		}
		void push(const T& val){
			_con.push_back(val);
		}
		void pop()
		{
			_con.pop_front();
		}
	private:
		Container _con;
	};

	void test_queue(){
		queue<int> q;
		q.push(1);
		q.push(2);
		q.push(3);
		q.push(4);

		cout << q.size() << endl;
		cout << q.back() << endl;
		cout << q.front() << endl;

		while (!q.empty()){
			cout << q.front() << " ";
			q.pop();
		}
		cout << endl;
		
	}
}