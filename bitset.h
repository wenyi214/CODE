#pragma once
//λͼ�Ͳ�¡������ֻ�ǿ������Ƿ���ڣ�һ��λ01״̬��0�����ڣ�1���ڡ���û�б�������
#include<vector>

class bitset{
	
public:
	bitset(size_t bitcount){
		_bitcount = bitcount;
		_bit.resize(bitcount / 32 + 1);//һ��intһ��32λ�����ռ��С���Ƕ��1���������ܲ�Ϊ0������������
	}

	int bitcount(){
		return _bitcount;
	}

	//�������ݣ�����Ӧλ��1
	void set(size_t num){

		if (num > _bitcount){
			return;
		}

		//�õ����ݵ�λ������_bit����ĸ��±�
		int index = num / 32;//����һ��Ԫ��32λ
		//�õ����������λ�õ���һλ
		int pos = num % 32;

		//����λ��1����λ�����
		_bit[index] |= (1 << pos);


	}

	//ɾ�����ݣ�����Ӧλ��0
	void reset(size_t num){
		if (num > _bitcount){
			return;
		}

		//�õ����ݵ�λ������_bit����ĸ��±�
		int index = num / 32;//����һ��Ԫ��32λ
		//�õ����������λ�õ���һλ
		int pos = num % 32;

		//��λ�����
		_bit[index] &= (~(1 << pos));
	}

	//������ݴ���
	bool test(size_t num){
		if (num > _bitcount){
			return false;
		}

		//�õ����ݵ�λ������_bit����ĸ��±�
		int index = num / 32;//����һ��Ԫ��32λ
		//�õ����������λ�õ���һλ
		int pos = num % 32;

		//��λ�����
		return (_bit[index] & (1 << pos)) != 0;
	}


private:
	std::vector<int> _bit;
	size_t _bitcount = 0;//λ���ܸ���
};