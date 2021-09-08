#pragma once

#include<iostream>
#include"BRTree.h"

namespace wy{
	template<class K,class T>
	class map{
		
	public:
		
		//仿函数来获取值
		struct MapKofT{
			const K& operator()(const pair<K, T>& k){
				return k.first;
			}
		};
		typedef typename  BRTree<K, pair<K, T>, MapKofT>::Iterator iterator;
		pair<iterator, bool> Insert(const pair<K, T>& data){
			return _tree.insert(data);
		}

		iterator begin(){
			return _tree.begin();
		}
		iterator end(){
			return _tree.end();
		}


		T& operator[](const K& k){
			pair<iterator, bool> ret = Insert(pair<K, T>(k, T()));
			return ret.first->second;
		}

		
	private:
		BRTree<K, pair<K, T>, MapKofT> _tree;//不是指针，用指针的话，刚开始树为空，访问不了树的结点。

	};


}