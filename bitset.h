#pragma once
//位图和布隆过滤器只是空数据是否存在，一个位01状态，0不存在，1存在。并没有保存数据
#include<vector>

class bitset{
	
public:
	bitset(size_t bitcount){
		_bitcount = bitcount;
		_bit.resize(bitcount / 32 + 1);//一个int一个32位。开空间大小总是多加1，余数可能不为0。补充余数。
	}

	int bitcount(){
		return _bitcount;
	}

	//保存数据，将对应位置1
	void set(size_t num){

		if (num > _bitcount){
			return;
		}

		//得到数据的位在数组_bit里的哪个下标
		int index = num / 32;//数组一个元素32位
		//得到保存数组该位置的哪一位
		int pos = num % 32;

		//将该位置1，按位或操作
		_bit[index] |= (1 << pos);


	}

	//删除数据，将对应位置0
	void reset(size_t num){
		if (num > _bitcount){
			return;
		}

		//得到数据的位在数组_bit里的哪个下标
		int index = num / 32;//数组一个元素32位
		//得到保存数组该位置的哪一位
		int pos = num % 32;

		//按位与操作
		_bit[index] &= (~(1 << pos));
	}

	//检查数据存在
	bool test(size_t num){
		if (num > _bitcount){
			return false;
		}

		//得到数据的位在数组_bit里的哪个下标
		int index = num / 32;//数组一个元素32位
		//得到保存数组该位置的哪一位
		int pos = num % 32;

		//按位或操作
		return (_bit[index] & (1 << pos)) != 0;
	}


private:
	std::vector<int> _bit;
	size_t _bitcount = 0;//位的总个数
};