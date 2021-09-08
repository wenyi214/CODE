#pragma once
#include<iostream>
#include<algorithm>
using namespace std;

//结点颜色
enum Color{
	BLACK,
	RED,
};

//结点
template<class T>
struct BRTreeNode
{
	BRTreeNode(const T& data)
	:_left(nullptr)
	, _right(nullptr)
	, _parent(nullptr)
	, _data(data)
	, _col(RED)//新增结点的颜色为红色
	{}

	BRTreeNode *_left;
	BRTreeNode *_right;
	BRTreeNode *_parent;

	T _data;//保存不同的值
	Color _col;//结点颜色
};

//迭代器实际是一个结点
template<class T,class Ref,class Ptr>
struct BRTreeIterator
{

	typedef BRTreeNode<T> Node;
	typedef BRTreeIterator<T, Ref, Ptr> Sef;

	//会默认生成拷贝构造，构造,可以是值拷贝，析构不需要释放空间
	BRTreeIterator(Node *node)
		:_node(node)
	{}

	//实现迭代器++
	Sef& operator++(){//不需要包含参数，直接用_node
		//右边右节点，找最左边的结点
		if (_node&&_node->_right){
			_node = _node->_right;
			while (_node->_left){
				_node = _node->_left;
			}

		}
		//右边没有结点，找右孩子不是cur的父亲。
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
		//右边存在，找左边最右边的结点
		if (_node->_left){
			_node = _node->_left;
			while (_node->_right){
				_node = _node->_right;
			}
			

		}
		//不存在，找左孩子不是_node的父亲
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
	//迭代器
	typedef BRTreeIterator<T, T&, T*> Iterator;
	
public:
	//不能用引用,return 的是匿名函数，生命周期只在那一行
	Iterator begin(){
		//最左节点
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
		//定义仿函数对象，作用于要取出值的结点，取出对应值
		KofT koft;
		//空结点，直接生成后，更新头节点。
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
			//此时肯定有grandfather，并且一定是黑色
			Node *grandfather = parent->_parent;
			//调整主要看uncle，下面判断uncle在哪边
			if (grandfather->_left == parent){//如果父亲在左边，
				Node *uncle = grandfather->_right;//叔叔就在右边
				//情况1：叔叔存在且为红色，变色就好了
				if (uncle&&uncle->_col == RED){
					parent->_col = BLACK;
					uncle->_col = BLACK;
					grandfather->_col = RED;

					//继续往上更新
					cur = grandfather;
					parent = cur->_parent;
				}
				//情况2：uncle不存在或者uncle为黑色，旋转+变色
				//两种情况，cur在parent的右边，左右双旋，cur在parent的右边，右单旋。
				else{
					//parent在grandfather的左边，如果cur在parent的右边，左右双旋，这里是先左单旋再右单旋
					if (cur == parent->_right){
						SigelLeft(parent);//parent成为cur的儿子了
						swap(cur, parent);//换回来，方便后面变色
					}
					//后面就都是cur在parent的右边，只有右单旋就好了
					SigelRight(grandfather);

					//变色处理，画图就理解怎么变色了
					grandfather->_col = RED;
					parent->_col = BLACK;


				}

			}
			else//uncle在左边
			{
				Node *uncle = grandfather->_left;
				//和上面一样，只是方向不一样
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

					//变色
					parent->_col = BLACK;
					grandfather->_col = RED;
				}

			}


		}

		_root->_col = BLACK;//防止变色将根节点变成红色
		return pair<Iterator, bool>(newnode, false);

	}
	

private:
	void SigelLeft(Node *parent){
		KofT koft;
		Node *subr = parent->_right;
		Node *subrl = subr->_left;

		Node *pparent = parent->_parent;
		parent->_right = subrl;
		if (subrl){//subrl可能为空
			subrl->_parent = parent;
		}

		subr->_left = parent;
		parent->_parent = subr;

		if (pparent == nullptr){//根节点
			subr->_parent = nullptr;
			_root = subr;
		}
		else{//子树
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





	
