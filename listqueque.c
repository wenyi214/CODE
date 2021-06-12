#include"listqueque.h"
//队列
void Quequeinit(Que *list)
{
	assert(list);
	//list->front = (Dlist)malloc(sizeof(struct Datalist));
	//list->end = (Dlist)malloc(sizeof(struct Datalist));
	list->front= NULL;//直接置空
	list->end = NULL;
	
}

static Dlist Nodelist(int x){//创建节点
	Dlist list = (Dlist)malloc(sizeof(struct Datalist));
	list->data = x;
	list->next = NULL;
	return list;
}

//要考虑没有元素情况
void Quelistpush(Que *list, int x)
{
	assert(list);
	//assert(node);
	Dlist ass = Nodelist(x);
	if (list->end == NULL&&list->front == NULL){
		list->front = ass;//是等于，实际front与end就是ass
		list->end = ass;//不是用next指向ass
	}
	else{
		list->end->next = ass;
		list->end = ass;
	}

}
//要考虑只有有一个元素的情况
Elementtype Quelistpop(Que *list)
{
	assert(list);
	Elementtype ass = 0;
	if (list->end == NULL&&list->front == NULL){
		printf("Empty\n");
		exit(-1);
	}
	else{
		Dlist temp;
		if (list->front->next == NULL){
			ass = list->front->data;
			list->end = NULL;
			list->front = NULL;

		}
		else{
			temp = list->front;
			ass = list->front->data;
			list->front = list->front->next;
			free(temp);
		}
	}
	return ass;


}
void Quelistdestory(Que *list){
	assert(list);
	Dlist temp;
	while (list->front){
		temp = list->front;
		list->front = list->front->next;
		free(temp);
	}
	list->end = NULL;
	list->front = NULL;

}

void Quelistprint(Que *list)
{
	assert(list);
	Dlist temp = list->front;//要用临时变量代替front，不是代替list
	if (list->end == NULL&&list->front == NULL){
		printf("Empty!\n");
		exit(-1);
	}
	else{
		while (temp){
			printf("%d ", temp->data);
			temp = temp->next;
		}
		printf("\n");
	}
}