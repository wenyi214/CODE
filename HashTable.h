#pragma once

#include<iostream>
#include<vector>
using namespace std;

namespace CLOSE_TABLE{

	enum State{
		EXIT,
		DELETE,
		EMPTY,
	};

	//kvģ��,KOFT��ȥ��key�ķº���
	template<class K, class T, class KofT>
	class HashTable
	{
	public:
		bool insert(const T& data){
			//����
			if (_ht.capacity() == 0 || _num * 10 / _ht.capacity() >= 7){
				int newcapacity = (_ht.capacity() == 0 ? 10 : 2 * _ht.capacity());
				HashTable<K, T, KofT> newht;
				//size����capacity
				newht._ht.resize(newcapacity);

				for (size_t i = 0; i < _ht.size(); i++){

					if (_ht[i]._state == EXIT){
						newht.insert(_ht[i]._data);
					}
				}
				_ht.swap(newht._ht);
				_num = newht._num;
			}
			KofT koft;
			int i = 1;
			int start = koft(data) % _ht.capacity();
			size_t index = start;
			while (_ht[index]._state != EMPTY){
				if (koft(_ht[index]._data) == koft(data)){
					return false;
				}
				//index++;
				index = (start + i*i) % _ht.capacity();
				i++;
				if (index >= _ht.capacity()){
					index = 0;
				}
			}
			_ht[index]._data = data;
			_ht[index]._state = EXIT;
			_num++;
			return true;
		}

		int find(const T& data){
			KofT koft;
			int index = koft(data) % _ht.capacity();
			if (koft(_ht[index]._data) != koft(data)){
				while (_ht[index]._state != EMPTY){
					index++;
					if (koft(_ht[index]._data) == koft(data)){

						return index;
					}
				}


			}
			return index;
		}

		bool erase(const T& data){
			KofT koft;
			int index = find(data);
			if (koft(_ht[index]._data) == koft(data)){
				_ht[index]._state = DELETE;
				_num--;
				return true;
			}
			return false;


		}

	public:
		struct Ele{
			T _data;
			State _state = EMPTY;
		};
	private:
		vector<Ele> _ht;//�����ﱣ��������ݺ�״̬
		size_t _num = 0;//Ԫ�ظ���
	};
}


namespace OPEN_TABLE{
	//���
	template<class T>
	struct HashNode{
		HashNode(const T& data)
		:_next(nullptr)
		, _data(data)
		{}
		T _data;
		HashNode *_next;
	};
	
	//װ���ε��࣬Ĭ�����Σ�ֱ�ӷ���
	template<class K>
	struct hash{
		int operator()(const K& k){
			return k;
		}
	};

	//�ػ�stringתint����
	template<>
	struct hash<string>
	{
		int operator()(const string& s){
			int hash = 0;
			for (size_t i = 0; i < s.size(); i++){
				hash *= 131;
				hash += s[i];
			}
			return hash;
		}
		

	};
	//��������ϣͰ�࣬�����ں��棬��Ҫ��ǰ���������������ſ���ʹ��
	template<class K, class T, class KOFT, class HASH >
	class HashTable;
	//������
	template<class K, class T, class KOFT, class HASH>//�����й�ϣͰָ�룬����Ҳ��Ҫ��ͬģ�����
	struct _Iterator{
		typedef HashNode<T> Node;
		typedef HashTable<K, T, KOFT, HASH> HashTable;
		typedef _Iterator<K, T, KOFT, HASH > Self;
		Node *_node;
		//��������һ����ϣͰ��ָ���ϣ��
		HashTable *_h;

		_Iterator(Node *node,HashTable *h)
			:_node(node)
			, _h(h)
		{}



		
		Self operator++(){
			HASH hash;
			KOFT koft;
			//��һ�������ڣ�������һ��λ�õ�����
			if (_node->_next){
				_node = _node->_next;
				return _Iterator(_node->_next, _h);
			}
			//��һ����㲻���ڣ����ع�ϣ���У�����һ����ϣͰ
			else{
				//�����ǰ�������ڹ�ϣ���е�λ��
				size_t index = hash(koft(_node->_data)) % _h->_ht.capacity();
				//��һ��λ��
				index++;
				for (; index < _h->_ht.capacity(); index++){
					if (_h->_ht[index]){
						_node = _h->_ht[index];
						return _Iterator(_node, _h);
						
					}
				}
				_node = nullptr;
				return _Iterator(_node, _h);
				
			}

		}

		T& operator*(){
			return _node->_data;
		}
		T* operator->(){
			return &(_node->_data);
		}

		bool operator!=(const Self& it){
			return _node != it._node;
		}
		bool operator==(const Self& it){
			return _node == it._node;
		}


	};

	


	template<class K,class T,class KOFT,class HASH>
	class HashTable{
		//����һ����ϣͰ����ʵ���ϣ��ĳ�Ա����(˽��)�����ó���Ԫ
		friend _Iterator<K, T, KOFT, HASH>;

		typedef HashNode<T> Node;
	public:
		//������
		typedef _Iterator<K, T, KOFT, HASH> Iterator;
		~HashTable(){
			clean();
			//vector���Լ�����
		}
		void clean(){
			for (size_t i = 0; i < _ht.capacity(); i++){
				Node *cur = _ht[i];
				if (cur){
					
					while (cur){
						Node *next = cur->_next;
						delete cur;
						cur = next;
					}
				}
				_ht[i] = nullptr;
			}
		}


		Iterator begin(){
			//�ҵ�һ�����ǿյĽ��
			Node *cur = nullptr;
			for (size_t i = 0; i < _ht.capacity(); i++){
				cur = _ht[i];
				if (cur){
					break;
				}
			}
			return Iterator(cur, this);
		}
		Iterator end(){
			return Iterator(nullptr, this);
		}


		pair<Iterator,bool> Insert(const T& data)
		{
			HASH hash;//ת����
			KOFT koft;//ȡ��key
			//�������
			if (_num == _ht.capacity()){

				//������
				int newcapacity = _ht.capacity() == 0 ? 10 : _ht.capacity() * 2;
				//������ָ�����飬����������ͷ�ڵ�
				vector<Node *> newht;
				newht.resize(newcapacity);
				//����������������㣬�ŵ���������
				for (size_t i = 0; i < _ht.capacity(); i++){
					Node *cur = _ht[i];
					while (cur){
						//����ȷ������λ�ã����Լ��ٳ�ͻ
						int index = hash(koft(cur->_data)) % newcapacity;
						//�����´�����㣬ֱ�ӽ��ɽ������������������
						Node *next = cur->_next;
						cur->_next = newht[index];
						newht[index] = cur;

						cur = next;

					}
					_ht[i] = nullptr;//��ֹҰָ��
				}
				//����Ҫ����_num��_numû�䣬HashTableû�䣬��������������
				_ht.swap(newht);

			}
			//����λ��
			int index = hash(koft(data)) % _ht.capacity();

			//����Ƿ���ڡ�
			Node *cur = _ht[index];
			while (cur){
				if (koft(cur->_data) == koft(data)){
					return pair<Iterator, bool>(Iterator(cur,this), false);
				}
				cur = cur->_next;
			}

			//������
			Node *newnode = new Node(data);
			newnode->_next = _ht[index];
			_ht[index] = newnode;
			_num++;

			return  pair<Iterator, bool>(Iterator(newnode,this), true);
			
		}

		Node *find(const T& data){
			HASH hash;
			KOFT koft;
			int index = hash(koft(data)) % _ht.capacity();

			Node *cur = _ht[index];
			while (cur){
				if (koft(cur->_data) == koft(data)){
					return cur;
				}


				cur = cur->_next;
			}

			return nullptr;
		}

		bool erase(const T& data){
			KOFT koft;
			HASH hash;
			
			//��λ��
			int index = hash(koft(data)) % _ht.capacity();

			Node *prev = nullptr;//����cur��ǰһ����㣬����ɾ��
			Node *cur = _ht[index];
			//�ҽ��
			while (cur&&koft(cur->_data) != koft(data)){
				prev = cur;
				cur = cur->_next;
			}
			//ɾ�����
			if (cur){
				if (prev){//����ͷ�ڵ�
					prev->_next = cur->_next;
				}
				else{//��ͷ�ڵ�
					_ht[index] = cur->_next;
				}
				delete cur;
			}
	
			
			return false;
			
		}
	private:
		vector<Node *> _ht;
		size_t _num = 0;
	};

}
