#include"BRTree.h"

namespace wy{
	template<class K>
	class set{
		
	public:

		
		//仿函数来获取值
		struct SetKofK{
			const K& operator()(const K& k){
				return k;
			}
		};

		typedef typename BRTree<K, K, SetKofK>::Iterator iterator;
		pair<iterator, bool> Insert(const K& k){
			return _tree.insert(k);
		}

		iterator begin(){
			return _tree.begin();
		}
		iterator end(){
			return _tree.end();
		}



	private:
		BRTree<K, K, SetKofK> _tree;
	};

}