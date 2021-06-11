#include"cirqueque.h"

void Quequeinit(Que *list)
{
	list->a = (Elementtype *)malloc(sizeof(Elementtype)* 12);
	list->capacity = 12;
	list->end = 0;
	list->front = 0;
}
void Quelistpush(Que *list, int x)
{
	assert(list);
	if ((list->end + 1) % list->capacity == list->front){
		printf("��������\n");
		exit(-1);
	}
	list->a[list->end] = x;
	//list->end++;����ѭ�����У������Ǽ򵥼�1
	list->end = (list->end + 1) % list->capacity;
}
Elementtype Quelistpop(Que *list)
{
	assert(list);
	if (list->end == list->front){
		printf("����Ϊ��\n");
		exit(-1);
	}
	Elementtype ass = 0;
	ass = list->a[list->front];
	list->front = (list->front + 1) % list->capacity;
	return ass;
}
void Quelistprint(Que *list)
{
	assert(list);
	if (list->end == list->front){
		printf("����Ϊ��\n");
		exit(-1);
	}
	for (int i = list->front; i < list->end; i++){
		printf("%d ", list->a[i]);
	}
	printf("\n");
}