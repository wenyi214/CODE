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
	//��ƶ�һ��λ�ã�����������
	pair<K, V> kv[M];
	//���ӽڵ�
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
		//ע����¸���
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
			//�ȷ�����
			_Inorder(root->child[i]);
			cout << root->kv[i].first<<" ";
		}
		//�ٷ������һ����
		_Inorder(root->child[i]);

		
	}

public:
	//���ӵ��������±����i
	//�Һ����������±���i+1
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
				//�ҵ����Ϊֹ���ڸ�λ�õ�������
				else if (cur->kv[i].first > key){
					break;
				}
				else{
					//�ҵ�
					return make_pair(cur, i);
				}
			}
			//
			cur = cur->child[i];
		}
		//û�ҵ�,������һ���ڵ�
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
			cout << "�Ѵ���" << endl;
			return false;
		}

		//���룬������
		Node *cur = ret.first;//����Ľڵ�
		pair<K, V> newkv = kv;//�����KV
		Node *sub = nullptr;//����ĺ��ӽڵ�
		//��cur����sub��newkv
		while (1){
			InserKV(cur, newkv, sub);

			if (cur->ksize < M){
				return true;
			}
			//��Ҫ����
			//�ֵܽڵ�
			Node *bro = new Node;
			//����һ�������
			size_t mid = M / 2;
			size_t j = 0;
			size_t i = mid + 1;
			
			for (; i < cur->ksize; i++){
				bro->ksize++;
				bro->kv[j] = cur->kv[i];
				//����Ҫ���ӽڵ㿽����ȥ
				bro->child[j] = cur->child[i];
				cur->child[i] = nullptr;
				cur->kv[i] = pair<K, V>();
				//ע����¸��׽ڵ�
				if (bro->child[j]){
					bro->child[j]->parent = bro;
				}
				j++;
			}
			//��ʣ���һ������
			bro->child[j] = cur->child[i];
			cur->child[i] = nullptr;

			if (bro->child[j]){
				bro->child[j]->parent = bro;
			}

			cur->ksize = mid;

			//1.û�и��ף�cur���Ǹ��������¸�
			//2.�и��ף��������ݺͺ���,�����ж��Ƿ���Ҫ����
			if (cur->parent == nullptr){
				//û�и��ڵ�
				//�����¸�
				root = new Node;
				root->kv[0] = cur->kv[mid];
				root->ksize = 1;
				cur->kv[mid] = pair<K, V>();
				//���¸��ڵ���ӽڵ�
				root->child[0] = cur;
				root->child[1] = bro;

				cur->parent = root;
				bro->parent = root;

				return true;
			}
			cur->kv[mid] = pair<K, V>();
			//�и��ڵ�,����bro��kv[mid]����ѭ�� 
			newkv = cur->kv[mid];
			cur = cur->parent;
			sub = bro;
		}



	}
	//�������
	void Inoeder()
	{
		_Inorder(root);
	}

};