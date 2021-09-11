#pragma once

#include"HashTable.h"

using namespace OPEN_TABLE;

namespace wy{
	template<class K, class T, class HASH = OPEN_TABLE::hash<K>>
	class unordered_map{
	private:
		struct MapKofT{
			const K& operator()(const pair<K,T>& data){
				return data.first;
			}
		};
		//typedef _Iterator<K, pair<K, T>, MapKofT, hash> iterator;
		typedef HashNode<T> Node;

	public:
		typedef typename HashTable<K, pair<K, T>, MapKofT, OPEN_TABLE::hash<K>>::Iterator iterator;
		iterator begin(){
			return _table.begin();
		}
		iterator end(){
			return _table.end();
		}
		T& operator[](const K& k){
			pair<iterator, bool> res = _table.Insert(make_pair(k, T()));
			return res.first->second;//resÊÇµü´úÆ÷->,µõÍ°µü´úÆ÷operator->
		}

		pair<iterator, bool> insert(const pair<K, T>& data){
			return _table.Insert(data);
		}


	private:
		HashTable<K, pair<K, T>, MapKofT, HASH> _table;

	};
}

