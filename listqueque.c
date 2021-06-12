#include"listqueque.h"
//����
void Quequeinit(Que *list)
{
	assert(list);
	//list->front = (Dlist)malloc(sizeof(struct Datalist));
	//list->end = (Dlist)malloc(sizeof(struct Datalist));
	list->front= NULL;//ֱ���ÿ�
	list->end = NULL;
	
}

static Dlist Nodelist(int x){//�����ڵ�
	Dlist list = (Dlist)malloc(sizeof(struct Datalist));
	list->data = x;
	list->next = NULL;
	return list;
}

//Ҫ����û��Ԫ�����
void Quelistpush(Que *list, int x)
{
	assert(list);
	//assert(node);
	Dlist ass = Nodelist(x);
	if (list->end == NULL&&list->front == NULL){
		list->front = ass;//�ǵ��ڣ�ʵ��front��end����ass
		list->end = ass;//������nextָ��ass
	}
	else{
		list->end->next = ass;
		list->end = ass;
	}

}
//Ҫ����ֻ����һ��Ԫ�ص����
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
	Dlist temp = list->front;//Ҫ����ʱ��������front�����Ǵ���list
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