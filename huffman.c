#include"Head.h"

typedef struct HTtree{
	int weight;
	struct HTtree *left;
	struct HTtree *right;
}Htree;



Htree *HuffmanTreeCreate(HD *head){

	int n = head->size-1;//Ԫ�ظ�����λʲôҪ��1
	for (int i = 0; i < n; i++){//ѭ����0��ʼ
		Htree *t = (Htree *)malloc(sizeof(Htree));
		//������С���ѣ��Ѷ�ԪΪ��Сֵ
		t->left = HeadPop(head);//ȡ���Ѷ�Ԫ�أ���Ϊ��������
		t->right = HeadPop(head);
		//������Ϊ��������Ȩֵ֮��
		t->weight = (t->left->weight) + (t->right->weight);
		//�������������
		HeadPush(head, t);
	}
	//���µ����һ����Ϊ��������
	return HeadPop(head);
}



