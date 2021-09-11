#pragma once
#include"HashTable.h"

using namespace OPEN_TABLE;

namespace wy{


	template<class K, class HASH = OPEN_TABLE::hash<K>>
	class unordered_set{
		
	private:
		struct SetKofT{
			const K& operator()(const K& k){
				return k;
			}
		};
		
	public:
		typedef typename HashTable<K, K, SetKofT, OPEN_TABLE::hash<K>>::Iterator iterator;
		iterator begin(){
			return _table.begin();
		}
		iterator end(){
			return _table.end();
		}

		pair<iterator, bool> insert(const K& k){
			return _table.Insert(k);
		}


	private:
		HashTable<K, K, SetKofT, HASH> _table;
	};
}