#pragma once
#include<iostream>
#include<algorithm>
using namespace std;

//�����ɫ
enum Color{
	BLACK,
	RED,
};

//���
template<class T>
struct BRTreeNode
{
	BRTreeNode(const T& data)
	:_left(nullptr)
	, _right(nullptr)
	, _parent(nullptr)
	, _data(data)
	, _col(RED)//����������ɫΪ��ɫ
	{}

	BRTreeNode *_left;
	BRTreeNode *_right;
	BRTreeNode *_parent;

	T _data;//���治ͬ��ֵ
	Color _col;//�����ɫ
};

//������ʵ����һ�����
template<class T,class Ref,class Ptr>
struct BRTreeIterator
{

	typedef BRTreeNode<T> Node;
	typedef BRTreeIterator<T, Ref, Ptr> Sef;

	//��Ĭ�����ɿ������죬����,������ֵ��������������Ҫ�ͷſռ�
	BRTreeIterator(Node *node)
		:_node(node)
	{}

	//ʵ�ֵ�����++
	Sef& operator++(){//����Ҫ����������ֱ����_node
		//�ұ��ҽڵ㣬������ߵĽ��
		if (_node&&_node->_right){
			_node = _node->_right;
			while (_node->_left){
				_node = _node->_left;
			}

		}
		//�ұ�û�н�㣬���Һ��Ӳ���cur�ĸ��ס�
		else{
			Node *parent = _node->_parent;
			while (parent&&parent->_right == _node){
				_node = parent;
				parent = parent->_parent;

			}
			_node = parent;
		}
		return *this;
	}
	Sef operator--(){
		//�ұߴ��ڣ���������ұߵĽ��
		if (_node->_left){
			_node = _node->_left;
			while (_node->_right){
				_node = _node->_right;
			}
			

		}
		//�����ڣ������Ӳ���_node�ĸ���
		else{
			Node *parent = _node->_parent;
			while (parent&&parent->_left == _node){
				_node = parent;
				parent = parent->_parent;
			}
			_node = parent;
		}
		return *this;
	}

	bool operator!=(const Sef& s){
		return _node != s._node;
	}
	bool operator==(const Sef& s){
		return _node == s._node;
	}
	Ref operator*()const{
		return _node->_data;
	}
	Ptr operator->()const{
		return &(_node->_data);
	}

	Node* _node = nullptr;
};



template<class K,class T,class KofT>
class BRTree
{
public:
	
	typedef BRTreeNode<T> Node;
	//������
	typedef BRTreeIterator<T, T&, T*> Iterator;
	
public:
	//����������,return ����������������������ֻ����һ��
	Iterator begin(){
		//����ڵ�
		Node *cur = _root;
		while (cur&&cur->_left){
			cur = cur->_left;
		}
		return Iterator(cur);
	}
	Iterator end(){
		return Iterator(nullptr);
	}



	pair<Iterator, bool> insert(const T& data){
		//����º�������������Ҫȡ��ֵ�Ľ�㣬ȡ����Ӧֵ
		KofT koft;
		//�ս�㣬ֱ�����ɺ󣬸���ͷ�ڵ㡣
		if (_root == nullptr){
			_root = new Node(data);
			_root->_col = BLACK;
			return pair<Iterator, bool>(_root, true);
		}
		Node *cur = _root;
		Node *parent = nullptr;
		while (cur){
			if (koft(data) > koft(cur->_data)){
				parent = cur;
				cur = cur->_right;
			}
			else if (koft(data) < koft(cur->_data)){
				parent = cur;
				cur = cur->_left;
			}
			else{
				return  pair<Iterator, bool>(cur, false);
			}
		}

		cur = new Node(data);
		cur->_parent = parent;
		if (koft(parent->_data)>koft(cur->_data)){
			parent->_left = cur;
		}
		else{
			parent->_right = cur;
		}
		Node *newnode = cur;
		while (parent&&parent->_col == RED){
			//��ʱ�϶���grandfather������һ���Ǻ�ɫ
			Node *grandfather = parent->_parent;
			//������Ҫ��uncle�������ж�uncle���ı�
			if (grandfather->_left == parent){//�����������ߣ�
				Node *uncle = grandfather->_right;//��������ұ�
				//���1�����������Ϊ��ɫ����ɫ�ͺ���
				if (uncle&&uncle->_col == RED){
					parent->_col = BLACK;
					uncle->_col = BLACK;
					grandfather->_col = RED;

					//�������ϸ���
					cur = grandfather;
					parent = cur->_parent;
				}
				//���2��uncle�����ڻ���uncleΪ��ɫ����ת+��ɫ
				//���������cur��parent���ұߣ�����˫����cur��parent���ұߣ��ҵ�����
				else{
					//parent��grandfather����ߣ����cur��parent���ұߣ�����˫�������������������ҵ���
					if (cur == parent->_right){
						SigelLeft(parent);//parent��Ϊcur�Ķ�����
						swap(cur, parent);//����������������ɫ
					}
					//����Ͷ���cur��parent���ұߣ�ֻ���ҵ����ͺ���
					SigelRight(grandfather);

					//��ɫ������ͼ�������ô��ɫ��
					grandfather->_col = RED;
					parent->_col = BLACK;


				}

			}
			else//uncle�����
			{
				Node *uncle = grandfather->_left;
				//������һ����ֻ�Ƿ���һ��
				if (uncle&&uncle->_col == RED){
					grandfather->_col = RED;
					parent->_col = BLACK;
					uncle->_col = BLACK;

					cur = grandfather;
					parent = cur->_parent;
				}
				else{
					if (cur == parent->_left){
						SigelRight(parent);
						swap(cur, parent);
					}
					SigelLeft(grandfather);

					//��ɫ
					parent->_col = BLACK;
					grandfather->_col = RED;
				}

			}


		}

		_root->_col = BLACK;//��ֹ��ɫ�����ڵ��ɺ�ɫ
		return pair<Iterator, bool>(newnode, false);

	}
	

private:
	void SigelLeft(Node *parent){
		KofT koft;
		Node *subr = parent->_right;
		Node *subrl = subr->_left;

		Node *pparent = parent->_parent;
		parent->_right = subrl;
		if (subrl){//subrl����Ϊ��
			subrl->_parent = parent;
		}

		subr->_left = parent;
		parent->_parent = subr;

		if (pparent == nullptr){//���ڵ�
			subr->_parent = nullptr;
			_root = subr;
		}
		else{//����
			subr->_parent = pparent;
			if (koft(pparent->_data) < koft(subr->_data)){
				
				pparent->_right = subr;
			}
			else{
				pparent->_left = subr;
			}

		}
	}
	void SigelRight(Node *parent){
		KofT koft;
		Node *subl = parent->_left;
		Node *sublr = subl->_right;

		Node *pparent = parent->_parent;

		parent->_left = sublr;
		if (sublr){
			sublr->_parent = parent;
		}

		subl->_right = parent;
		parent->_parent = subl;

		if (pparent == nullptr){
			subl->_parent = nullptr;
			_root = subl;
		}
		else{
			subl->_parent = pparent;

			if (koft(pparent->_data) < koft(subl->_data)){
				pparent->_right = subl;
			}
			else{
				pparent->_left = subl;
			}
		}


	}


private:
	Node *_root = nullptr;

};





	
