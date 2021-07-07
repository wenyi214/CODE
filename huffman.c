#include"Head.h"

typedef struct HTtree{
	int weight;
	struct HTtree *left;
	struct HTtree *right;
}Htree;



Htree *HuffmanTreeCreate(HD *head){

	int n = head->size-1;//元素个数，位什么要减1
	for (int i = 0; i < n; i++){//循环从0开始
		Htree *t = (Htree *)malloc(sizeof(Htree));
		//建立的小根堆，堆顶元为最小值
		t->left = HeadPop(head);//取出堆顶元素，作为左右子树
		t->right = HeadPop(head);
		//新树根为左右子树权值之和
		t->weight = (t->left->weight) + (t->right->weight);
		//将新树放入堆中
		HeadPush(head, t);
	}
	//留下的最后一颗树为哈夫曼树
	return HeadPop(head);
}



