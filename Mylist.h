#include<iostream>
#include<assert.h>

using namespace std;

namespace my{
	//�����Ϣ
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

	//������
	//Ϊ��ͬʱʵ��iterator��const_iterator,ģ�庬����������
	template<class T, class Ref, class Ptr>
	struct _iterator{
		typedef _iterator<T, Ref, Ptr> Self;
		typedef __list_node<T> Node;
		//��Ա����
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
		//ǰ��++
		Self& operator++(){
			_node = _node->_next;
			return *this;
		}
		//����++
		Self operator++(int){
			Self tmp(_node);
			_node = _node->_next;
			return tmp;
		}
		//ǰ��--
		Self& operator--(){
			_node = _node->_prev;
			return *this;
		}
		//����--
		Self operator--(int){
			Self tmp(_node);
			_node = _node->_prev;
			return tmp;
		}

	};

	template<class T>
	class _list{
	public:
		//const������
		typedef _iterator<T, const T&, const T*> const_iterator;
		//��ͨ������
		typedef _iterator<T, T&, T*> iterator;
		typedef __list_node<T> Node;
	public:
		//���캯����ΪҪ������������Ҫ��ͷ�ڵ㶯̬����ռ�
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
			//�Ȱѽ��ɾ�ˣ���ɾͷ�ڵ�
			clear();
			delete _head;
			_head = nullptr;
		}
		//���ص�������ʼ�ͽ�β��д��ͼ
		//����ֱ�ӷ���_head->next,������ǵ���������
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
			//�Ƚ���������ʣ��һ��ͷ�ڵ�
			clear();
			//��������һ��
			_list<T> tmp(lt);
			swap(_head, tmp._head);
			return *this;
		}
		void clear(){
			iterator it = begin();
			while (it != end()){

				erase(it++);
				//������ʧЧ
				//it++;
			}
			
		}
		void push_back(const T& val){

			//Node* tail = _head->_prev;
			//����һ���½ڵ�
			//Node* newnode = new Node(val);

			//tail->_next = newnode;
			//newnode->_next = _head;
			//newnode->_prev = tail;
			//_head->_prev = newnode;
			//������������
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
			//�õ�pos��Ľ��
			Node* cur = pos._node;
			Node* newnode = new Node(val);

			Node* prev = cur->_prev;
			

			prev->_next = newnode;
			newnode->_prev = prev;
			newnode->_next = cur;
			cur->_prev = newnode;
			//���ص��������õ���������һ��
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
			//�����޸�
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