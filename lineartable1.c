#include<stdio.h>
#include<Windows.h>
#pragma warning(disable:4996)
#define MAX 100

struct Lnode{
	int date[MAX];
	int last;//data���ݳ���Ϊlast+1.
};
typedef struct Lnode* LIST;

LIST Inittable(){
	LIST L = (LIST)malloc(sizeof(struct Lnode));//����һ��
	L->last = -1;
	return L;
}
//�������������һ��
void Insert(LIST list, int x, int i){
	if (list->last == MAX - 1){
		printf("���Ա����ˣ�\n");
	}
	if (i<1 || i>(list->last + 2)){//�ܹ�last+1��Ҫ���룬���ټ�һ��
		printf("������Χ��\n");
	}
	for (int j = list->last; j >= i - 1; j--){//Ϊʲôj>=i-1,���飬�±��0��ʼ����ΪҪ��last��i-1�����ƿ�
		list->date[j + 1] = list->date[j];
	}
	list->date[i - 1] = x;
	list->last += 1;
}

int Find(LIST list, int x){//���ص�һ�γ���x���±�
	int i = 0;
	while (i <= list->last&&list->date[i] != x){//��ֹ���������С
		i++;
	}
	if (i > list->last){
		printf("û�ҵ�\n");
		return 0;
	}
	else{
		return i + 1;
	}

}
//�������ǰ��
void Delet(LIST list, int i){
	if (i<1 || i>list->last + 1){
		printf("������\n");
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
