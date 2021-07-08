#include<stdio.h>
#include<stdlib.h>

typedef int ElementType;

typedef struct Balancetree{
	ElementType data;
	struct Balancetree *left;
	struct Balancetree *right;

}Btree;

Btree *CreateNode(ElementType x){
	Btree *node = (Btree *)malloc(sizeof(Btree));
	node->data = x;
	node->left = NULL;
	node->right = NULL;
	return node;
}
int GetHight(Btree *tree){
	if (!tree){
		return 0;
	}
	int lh = GetHight(tree->left);
	int rh = GetHight(tree->right);
	int h = lh > rh ? lh : rh;
	return h + 1;
}

//����
void SigleLeftCir(Btree **tree){
	Btree *a = *tree;
	Btree *b = (*tree)->right;
	a->right = b->left;
	b->left = a;
	(*tree) = b;

}

//����
void SigleRightCir(Btree **tree){
	Btree *a = *tree;
	Btree *b = (*tree)->left;
	a->left = b->right;
	b->right = a;
	(*tree) = b;

}

//������
void RightLeftCir(Btree **tree){
	SigleRightCir(&(*tree)->right);
	SigleLeftCir(tree);

}

//������
void LeftRightCir(Btree **tree){
	SigleLeftCir(&(*tree)->left);
	SigleRightCir(tree);

}



Btree *BalanceTreeInsert(Btree **tree, ElementType x){
	if ((*tree) == NULL){
		(*tree) = CreateNode(x);
	}
	else{
		if ((*tree)->data < x){
			(*tree)->right = BalanceTreeInsert(&(*tree)->right, x);

			if (GetHight((*tree)->left) - GetHight((*tree)->right) == -2){
				if ((*tree)->right->data < x){
					SigleLeftCir((tree));
				}
				else{
					RightLeftCir(tree);
				}
			}
		}
		else if ((*tree)->data>x){
			(*tree)->left = BalanceTreeInsert(&(*tree)->left, x);

			if (GetHight((*tree)->left) - GetHight((*tree)->right) == 2){
				if ((*tree)->data < x){
					SigleRightCir(tree);
				}
				else{
					LeftRightCir(tree);
				}
			}
		}
		else{
			printf("�Ѵ���\n");
			exit(-1);
		}
	}
	return *tree;


}


Btree *Getmax(Btree *tree){
	while (tree->right){
		tree = tree->right;
	}
	return tree;
}

Btree *BalanceTreeDelet(Btree **tree, ElementType x){
	if ((*tree) == NULL){
		printf("������\n");
		exit(-1);
	}
	if ((*tree)->data < x){
		//�ݹ鷵��˵��ɾ����
		(*tree)->right = BalanceTreeDelet(&(*tree)->right, x);
		//�ж�
		if (GetHight((*tree)->left) - GetHight((*tree)->right) == 2){
			//���ӽ��
			if (GetHight((*tree)->left->left) - GetHight((*tree)->left->right) == 1){
				SigleRightCir(tree);
			}
			else{
				LeftRightCir(tree);
			}
		}


	}
	else if ((*tree)->data > x){
		(*tree)->left = BalanceTreeDelet(&(*tree)->left, x);
		//�ж�
		if (GetHight((*tree)->left) - GetHight((*tree)->right) == -2){
			//���ӽ��
			if (GetHight((*tree)->right->left) - GetHight((*tree)->right->right) == -1){
				SigleLeftCir(tree);
			}
			else{
				RightLeftCir(tree);
			}
		}

	}
	//�ҵ�
	else{
		if ((*tree)->left && (*tree)->right){
			Btree *temp = Getmax((*tree)->left);
			int x = temp->data;
			(*tree)->data = x;
			(*tree)->left = BalanceTreeDelet(&(*tree)->left, x);
			//����Ҳ��Ҫ����
			if (GetHight((*tree)->left) - GetHight((*tree)->right) == -2){
				//���ӽ��
				if (GetHight((*tree)->right->left) - GetHight((*tree)->right->right) == -1){
					SigleLeftCir(tree);
				}
				else{
					RightLeftCir(tree);
				}
			}

		}
		else if ((*tree)->left == NULL && (*tree)->right){
			Btree *temp = *tree;
			*tree = (*tree)->right;
			free(temp);

		}
		else if ((*tree)->left == NULL && (*tree)->right){
			Btree *temp = *tree;
			*tree = (*tree)->left;
			free(temp);

		}
		else{
			Btree *temp = *tree;
			*tree = NULL;
			free(temp);
		}
	}
	return *tree;
}



int main(){
	Btree *root = NULL;
	BalanceTreeInsert(&root, 6);
	BalanceTreeInsert(&root, 4);
	BalanceTreeInsert(&root, 9);
	BalanceTreeInsert(&root, 2);
	BalanceTreeInsert(&root, 3);
	BalanceTreeInsert(&root, 5);
	BalanceTreeInsert(&root, 10);
	BalanceTreeInsert(&root, 8);


	BalanceTreeDelet(&root, 6);
	return 0;
}