#pragma once

#include<iostream>
using namespace std;

template<class K,class V>
struct BSTreeNode{
	BSTreeNode(K key,V value)
	:_left(nullptr)
	, _right(nullptr)
	, _key(key)
	, _value(value)
	{}
	BSTreeNode *_left;
	BSTreeNode *_right;
	K _key;
	V _value;
};

template<class K,class V>
class KVBSTtree
{
	typedef BSTreeNode<K,V> Node;
public:
	bool Insert(K key, V value){
		if (_root == nullptr){
			_root = new Node(key, value);
			return true;
		}
		Node *cur = _root;
		Node *curparent = nullptr;
		while (cur){
			if (key > cur->_key){
				curparent = cur;
				cur = cur->_right;
			}
			else if (key < cur->_key){
				curparent = cur;
				cur = cur->_left;
			}
			else{
				cout << "�Ѵ���" << endl;
				return false;
			}
		}
		//�ҵ�����λ��
		cur = new Node(key, value);
		//�жϲ��븸�׵���߻����ұ�
		if (curparent->_key < key){
			curparent->_right = cur;
		}
		else if (curparent->_key>key){
			curparent->_left = cur;
		}
		else{

		}
		return true;

	}

	Node *Find(K key){
		if (_root == nullptr){
			//cout << "û�ҵ�" << endl;
			return nullptr;
		}
		Node *cur = _root;
		while (cur){
			if (key > cur->_key){
				cur = cur->_right;
			}
			else if (key < cur->_key){
				cur = cur->_left;
			}
			else{
				return cur;
			}
		}
		//û�ҵ�
		return nullptr;
	}

	bool Erase(K key){
		if (_root == nullptr){
			return false;
		}
		Node *curparent = nullptr;
		Node *cur = _root;
		while (cur){
			if (key > cur->_key){
				curparent = cur;
				cur = cur->_right;
			}
			else if (key < cur->_key){
				curparent = cur;
				cur = cur->_left;
			}
			else{
				if (cur->_left == nullptr&&cur->_right == nullptr){
					//ֻ��һ���ڵ�
					if (curparent == nullptr){
						delete cur;
						_root = nullptr;
						return true;
					}
					
					if (curparent->_left == cur){
						curparent->_left = nullptr;
					}
					else{
						curparent->_right = nullptr;
					}
					//��ɾ��������ͷţ����ͷż��Ҳ�����
					delete cur;
				}
				else if (cur->_left == nullptr&&cur->_right){
					if (curparent->_left == cur){
						curparent->_left = cur->_right;
					}
					else{
						curparent->_right = cur->_right;
					}
					delete cur;
				}
				else if (cur->_left &&cur->_right == nullptr){
					if (curparent->_left == cur){
						curparent->_left = cur->_left;
					}
					else{
						curparent->_right = cur->_left;
					}
					delete cur;
				}
				else{
					Node *Minparent = cur;
					//����������Сֵ
					Node *tail = cur->_right;
					while (tail->_left){
						Minparent = tail;
						tail=tail->_left;
					}
					K tmpkey = tail->_key;
					V tmpvalue = tail->_value;
					cur->_key = tmpkey;
					cur->_value = tmpvalue;
					//ɾ����������Сֵ
					if (Minparent->_left == tail){
						Minparent->_left = tail->_right;
					}
					else{
						Minparent->_right = tail->_right;
					}
					delete tail;

				}
				return true;


			}
		

		}
		return false;

	}
	void _InNode(Node *root){
		if (root){
			_InNode(root->_left);
			cout << root->_key << "->" << root->_value << ' ';
			_InNode(root->_right);
		}
	}

	void InNode(){
		_InNode(_root);
		cout << endl;
	}

private:
	Node *_root = nullptr;
};
