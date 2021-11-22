#pragma once
#include<iostream>
using namespace std;

template<class K, class V, size_t M>
struct BtreeNode{

	BtreeNode()
		:parent(nullptr)
		,ksize(0)
	{
		for (int i = 0; i < M + 1; i++){
			child[i] = nullptr;
		}
	}
	//设计多一个位置，方便最后插入
	pair<K, V> kv[M];
	//父子节点
	BtreeNode<K, V, M> *child[M + 1];
	BtreeNode<K, V, M> *parent;

	size_t ksize;
};

template<class K, class V, size_t M>
class Btree{
	typedef BtreeNode<K, V, M> Node;
private:
	Node *root = nullptr;


	void InserKV(Node *cur, pair<K, V> kv, Node *sub){
		int i = cur->ksize - 1;
		while (i >= 0){
			if (cur->kv[i].first <= kv.first){
				break;
			}
			cur->kv[i + 1] = cur->kv[i];
			cur->child[i + 2] = cur->child[i + 1];
			i--;
		}

		cur->kv[i + 1] = kv;
		cur->child[i + 2] = sub;
		cur->ksize++;
		//注意更新父亲
		if (sub){
			sub->parent = cur;
		}
	}

	void _Inorder(Node *root){
		if (root == nullptr){
			return;
		}
		size_t i = 0;
		for (; i < root->ksize; i++){
			//先访问左
			_Inorder(root->child[i]);
			cout << root->kv[i].first<<" ";
		}
		//再访问最后一个右
		_Inorder(root->child[i]);

		
	}

public:
	//左孩子等于数据下标等于i
	//右孩子是数据下标是i+1
	pair<Node*, int> find(const K& key){
		Node *cur = root;
		Node *parent = nullptr;
		while (cur){
			parent = cur;
			size_t i = 0;
			while (i<cur->ksize){
				if (cur->kv[i].first < key){
					i++;
				}
				//找到大的为止，在该位置的左孩子里
				else if (cur->kv[i].first > key){
					break;
				}
				else{
					//找到
					return make_pair(cur, i);
				}
			}
			//
			cur = cur->child[i];
		}
		//没找到,返回上一个节点
		return make_pair(parent, -1);
	}

	bool Insert(const pair<K, V>& kv){
		if (root == nullptr){
			root = new Node;

			root->kv[0] = kv;
			root->ksize = 1;
			return true;
		}

		pair<Node *, int> ret = find(kv.first);

		if (ret.second >= 0){
			cout << "已存在" << endl;
			return false;
		}

		//插入，不存在
		Node *cur = ret.first;//插入的节点
		pair<K, V> newkv = kv;//插入的KV
		Node *sub = nullptr;//插入的孩子节点
		//往cur插入sub和newkv
		while (1){
			InserKV(cur, newkv, sub);

			if (cur->ksize < M){
				return true;
			}
			//需要分裂
			//兄弟节点
			Node *bro = new Node;
			//拷贝一半的数据
			size_t mid = M / 2;
			size_t j = 0;
			size_t i = mid + 1;
			
			for (; i < cur->ksize; i++){
				bro->ksize++;
				bro->kv[j] = cur->kv[i];
				//还需要将子节点拷贝过去
				bro->child[j] = cur->child[i];
				cur->child[i] = nullptr;
				cur->kv[i] = pair<K, V>();
				//注意更新父亲节点
				if (bro->child[j]){
					bro->child[j]->parent = bro;
				}
				j++;
			}
			//还剩最后一个孩子
			bro->child[j] = cur->child[i];
			cur->child[i] = nullptr;

			if (bro->child[j]){
				bro->child[j]->parent = bro;
			}

			cur->ksize = mid;

			//1.没有父亲，cur就是根，产生新根
			//2.有父亲，插入数据和孩子,继续判断是否需要分裂
			if (cur->parent == nullptr){
				//没有父节点
				//创建新根
				root = new Node;
				root->kv[0] = cur->kv[mid];
				root->ksize = 1;
				cur->kv[mid] = pair<K, V>();
				//更新父节点和子节点
				root->child[0] = cur;
				root->child[1] = bro;

				cur->parent = root;
				bro->parent = root;

				return true;
			}
			cur->kv[mid] = pair<K, V>();
			//有父节点,插入bro和kv[mid]利用循环 
			newkv = cur->kv[mid];
			cur = cur->parent;
			sub = bro;
		}



	}
	//中序遍历
	void Inoeder()
	{
		_Inorder(root);
	}

};