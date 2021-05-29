#include<stdio.h>
#include<Windows.h>
#pragma warning(disable:4996)

struct Node{
	int num;
	struct Node *next;
};


struct Node* Creatlist(){//创建链表头
	struct Node *headnode = (struct Node*)malloc(sizeof(struct Node));
	headnode->next = NULL;
	return headnode;
}

struct Node* Creatnode(int num){//创建结点，链表,参数数字域
	struct Node* newnode = (struct Node*)malloc(sizeof(struct Node));
	newnode->num = num;
	newnode->next = NULL;
	return newnode;
}

void Insetlist(struct Node* list, int num){//头插法
	struct Node* insetnode = Creatnode(num);
	if (list != NULL){
		insetnode->next = list->next;
		list->next = insetnode;
	}
	else{
		printf("节点不存在\n");
	}
}
void Insetlists(struct Node* headnode, int n, int num){//在n节点处插入,参数头节点，在第n个节点处插，插入的数据num
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
		printf("节点不存在\n");
	}
}
void Printnode(struct Node* headnode){//打印节点
	struct Node* list = headnode;
	while ((list->next) != NULL){
		list = list->next;
		printf("%d\t",  list->num);
	}
	printf("\n");

}

void Modifynode(struct Node* headnode, int n){//修改节点，参数链表，修改的第n个节点
	struct Node* list = headnode;
	int i = 0;
	while (i < n&&list != NULL){
		list = list->next;
		i++;
	}
	if (list != NULL){
		printf("请输入你要修改的值\n");
		int j = 0;
		scanf("%d", &j);
		list->num = j;

	}
	else{
		printf("节点不存在\n");
	}
}

void Deletnode(struct Node* headnode, int n){//删除第n个节点，
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
		printf("节点不存在\n");
	}
}

int main(){
	struct Node* list = Creatlist();

	Insetlists(list, 1, 1);
	Printnode(list);
	int i = 0;
	printf("请输入修改哪个节点\n");
	scanf("%d", &i);
	Modifynode(list, i);
	Printnode(list);
	printf("请输入删除哪个节点\n");
	int n = 0;
	scanf("%d", &n);
	Deletnode(list, n);
	Printnode(list);
	system("pause");
	return 0;
}