#include<iostream>
#include<assert.h>

using namespace std;

namespace my{
	//结点信息
	template<class T>
	struct __list_node{
		__list_node(const T& val = T())
		:_next(nullptr)
		, _prev(nullptr)
		, _val(val)
		{}
		__list_node* _prev;
		__list_node* _next;
		T _val;
	};

	//迭代器
	//为了同时实现iterator和const_iterator,模板含有三个变量
	template<class T, class Ref, class Ptr>
	struct _iterator{
		typedef _iterator<T, Ref, Ptr> Self;
		typedef __list_node<T> Node;
		//成员变量
		Node* _node;

		_iterator(Node* node=nullptr)
			:_node(node)
		{}
		_iterator(const Self& it)
			:_node(it._node)
		{}

		Ref operator*(){
			return _node->_val;
		}
		Ptr operator->(){
			return &_node->_val;
		}

		bool operator==(const _iterator& x)const{
			return _node == x._node;
		}
		bool operator!=(const _iterator& x)const{
			return _node != x._node;
		}
		//前置++
		Self& operator++(){
			_node = _node->_next;
			return *this;
		}
		//后置++
		Self operator++(int){
			Self tmp(_node);
			_node = _node->_next;
			return tmp;
		}
		//前置--
		Self& operator--(){
			_node = _node->_prev;
			return *this;
		}
		//后置--
		Self operator--(int){
			Self tmp(_node);
			_node = _node->_prev;
			return tmp;
		}

	};

	template<class T>
	class _list{
	public:
		//const迭代器
		typedef _iterator<T, const T&, const T*> const_iterator;
		//普通迭代器
		typedef _iterator<T, T&, T*> iterator;
		typedef __list_node<T> Node;
	public:
		//构造函数因为要构建对象，所以要给头节点动态申请空间
		_list(){
			_head = new Node;
			_head->_next = _head;
			_head->_prev = _head;
		}
		_list(int n, const T& val){
			_head = new Node;
			_head->_next = _head;
			_head->_prev = _head;

			while (n){
				push_back(val);
				n--;
			}
		}

		template<class inputiterator>
		_list(inputiterator start, inputiterator end){
			_head = new Node;
			_head->_next = _head;
			_head->_prev = _head;

			while (start != end){
				push_back(*start);
				start++;
			}
		}

		_list(const _list<T>& lt){
			_head = new Node;
			_head->_next = _head;
			_head->_prev = _head;

			const_iterator it = lt.begin();
				while (it != lt.end()){
					push_back(*it);
					it++;
				}
			

			//for (auto c : lt){
			//	push_back(c);
			//}
		}
		~_list(){
			//先把结点删了，再删头节点
			clear();
			delete _head;
			_head = nullptr;
		}
		//返回迭代器开始和结尾。写画图
		//不能直接返回_head->next,这个不是迭代器类型
		iterator begin(){
			return iterator(_head->_next);
			//return _head->_next;
		}
		iterator end(){
			return iterator(_head);
		}
		const_iterator begin()const{
			return const_iterator(_head->_next);
		}
		const_iterator end()const{
			return const_iterator(_head);
		}

		_list<T>& operator=(_list<T>& lt){
			//先将结点清楚，剩下一个头节点
			clear();
			//拷贝构造一个
			_list<T> tmp(lt);
			swap(_head, tmp._head);
			return *this;
		}
		void clear(){
			iterator it = begin();
			while (it != end()){

				erase(it++);
				//迭代器失效
				//it++;
			}
			
		}
		void push_back(const T& val){

			//Node* tail = _head->_prev;
			//创建一个新节点
			//Node* newnode = new Node(val);

			//tail->_next = newnode;
			//newnode->_next = _head;
			//newnode->_prev = tail;
			//_head->_prev = newnode;
			//传迭代器参数
			insert(end(), val);

		}

		void pop_back(){

			erase(end()--);
		}
		void push_front(const T& val){
			insert(begin(), val);

		}
		void pop_front(){
			erase(begin());
		}

		iterator insert(iterator pos, const T& val){
			//拿到pos里的结点
			Node* cur = pos._node;
			Node* newnode = new Node(val);

			Node* prev = cur->_prev;
			

			prev->_next = newnode;
			newnode->_prev = prev;
			newnode->_next = cur;
			cur->_prev = newnode;
			//返回迭代器，用迭代器构造一个
			return iterator(newnode);
		}
		iterator erase(iterator pos){
			assert(pos != end());
			Node* cur = pos._node;
			Node* next = cur->_next;
			Node* prev = cur->_prev;

			prev->_next = next;
			next->_prev = prev;
			delete cur;
			
			return iterator(next);
		}
		size_t size(){
			size_t len = 0;
			for (auto c : *this){
				len++;
			}
			return len;
		}
		bool empty(){
			return _head->_next == _head;
		}

	private:
		Node* _head;
	};
	//template<class T>
	void Print(const _list<int>& lt){
		auto it = lt.begin();
		while (it != lt.end()){
			//不可修改
			//*it = 1;
			cout << *it << " ";
			it++;
		}
		cout << endl;
	}

	void test_list1(){
		_list<int> lt1;
		lt1.push_back(1);
		lt1.push_back(2);
		lt1.push_back(3);
		lt1.push_back(4);
		Print(lt1);

		_list<int> lt2(lt1);
		Print(lt2);

		_list<int> lt3(lt1.begin(), lt1.end());
		Print(lt3);

		_list<int> lt4(4, 1);
		Print(lt4);
	}

	void test_list2(){
		_list<int> lt1;
		lt1.push_back(1);
		lt1.push_back(2);
		lt1.push_back(3);
		lt1.push_back(4);

		lt1.push_back(5);
		lt1.push_back(6);
		Print(lt1);
		lt1.pop_back();
		Print(lt1);

		lt1.push_front(0);
		lt1.push_front(-1);
		Print(lt1);

		lt1.pop_front();
		Print(lt1);

	}
	void test_list3(){
		_list<int> lt1;
		lt1.push_back(1);
		lt1.push_back(2);
		lt1.push_back(3);
		lt1.push_back(4);
		Print(lt1);

		_list<int> lt2;
		lt2.push_back(10);
		lt2.push_back(20);
		lt2.push_back(30);
		lt2.push_back(40);
		lt1 = lt2;
		Print(lt2);
		Print(lt1);
		cout << lt1.size() << endl;
		lt2.clear();
		cout << lt1.empty() << endl;
		cout << lt2.empty() << endl;


	}
};