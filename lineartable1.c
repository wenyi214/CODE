#include<stdio.h>
#include<Windows.h>
#pragma warning(disable:4996)
#define MAX 100

struct Lnode{
	int date[MAX];
	int last;//data数据长度为last+1.
};
typedef struct Lnode* LIST;

LIST Inittable(){
	LIST L = (LIST)malloc(sizeof(struct Lnode));//创建一个
	L->last = -1;
	return L;
}
//将后面的往后移一格，
void Insert(LIST list, int x, int i){
	if (list->last == MAX - 1){
		printf("线性表满了！\n");
	}
	if (i<1 || i>(list->last + 2)){//总共last+1，要插入，得再加一个
		printf("超过范围！\n");
	}
	for (int j = list->last; j >= i - 1; j--){//为什么j>=i-1,数组，下标从0开始，因为要将last到i-1的数移开
		list->date[j + 1] = list->date[j];
	}
	list->date[i - 1] = x;
	list->last += 1;
}

int Find(LIST list, int x){//返回第一次出现x的下标
	int i = 0;
	while (i <= list->last&&list->date[i] != x){//防止超过数组大小
		i++;
	}
	if (i > list->last){
		printf("没找到\n");
		return 0;
	}
	else{
		return i + 1;
	}

}
//后面的往前移
void Delet(LIST list, int i){
	if (i<1 || i>list->last + 1){
		printf("不存在\n");
	}
	for (int j = i - 1; j < list->last; j++){
		list->date[j] = list->date[j + 1];
	}
	list->last -= 1;
}
void Modify(LIST list, int x, int i){
	list->date[i - 1] = x;
}



void Print(LIST list){
	for (int i = 0; i < list->last+1; i++){
		printf("%d ", list->date[i]);
	}
	printf("\n");
}




int main(){
	LIST L = Inittable();
	Insert(L, 2, 1);
	Insert(L, 4, 2);
	Modify(L, 4, 1);
	//Delet(L, 1);
	//int p = Find(L, 4);
	//printf("%d", p);
	Print(L);
	system("pause");
	return 0;
}
