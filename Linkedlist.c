#include<stdio.h>
#include<Windows.h>
#pragma warning(disable:4996)

struct Node{
	int num;
	struct Node *next;
};


struct Node* Creatlist(){//��������ͷ
	struct Node *headnode = (struct Node*)malloc(sizeof(struct Node));
	headnode->next = NULL;
	return headnode;
}

struct Node* Creatnode(int num){//������㣬����,����������
	struct Node* newnode = (struct Node*)malloc(sizeof(struct Node));
	newnode->num = num;
	newnode->next = NULL;
	return newnode;
}

void Insetlist(struct Node* list, int num){//ͷ�巨
	struct Node* insetnode = Creatnode(num);
	if (list != NULL){
		insetnode->next = list->next;
		list->next = insetnode;
	}
	else{
		printf("�ڵ㲻����\n");
	}
}
void Insetlists(struct Node* headnode, int n, int num){//��n�ڵ㴦����,����ͷ�ڵ㣬�ڵ�n���ڵ㴦�壬���������num
	int i = 1;
	struct Node *t = headnode;
	while (i < n&& t!= NULL){
		t = t->next;
		i++;
	}
	if (t != NULL){
		struct Node* insetnode = Creatnode(num);
		insetnode->next = t->next;
		t->next = insetnode;
	}
	else{
		printf("�ڵ㲻����\n");
	}
}
void Printnode(struct Node* headnode){//��ӡ�ڵ�
	struct Node* list = headnode;
	while ((list->next) != NULL){
		list = list->next;
		printf("%d\t",  list->num);
	}
	printf("\n");

}

void Modifynode(struct Node* headnode, int n){//�޸Ľڵ㣬���������޸ĵĵ�n���ڵ�
	struct Node* list = headnode;
	int i = 0;
	while (i < n&&list != NULL){
		list = list->next;
		i++;
	}
	if (list != NULL){
		printf("��������Ҫ�޸ĵ�ֵ\n");
		int j = 0;
		scanf("%d", &j);
		list->num = j;

	}
	else{
		printf("�ڵ㲻����\n");
	}
}

void Deletnode(struct Node* headnode, int n){//ɾ����n���ڵ㣬
	int i = 1;
	struct Node *strat = headnode;
	struct Node *end = headnode->next;

	while (i < n&&end != NULL){
		strat = strat->next;
		end = end->next;
		i++;
	}
	if (end != NULL){
		strat->next = end->next;
		free(end);

	}
	else{
		printf("�ڵ㲻����\n");
	}
}

int main(){
	struct Node* list = Creatlist();

	Insetlists(list, 1, 1);
	Printnode(list);
	int i = 0;
	printf("�������޸��ĸ��ڵ�\n");
	scanf("%d", &i);
	Modifynode(list, i);
	Printnode(list);
	printf("������ɾ���ĸ��ڵ�\n");
	int n = 0;
	scanf("%d", &n);
	Deletnode(list, n);
	Printnode(list);
	system("pause");
	return 0;
}