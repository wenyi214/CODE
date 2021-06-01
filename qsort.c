#include<stdio.h>
#include<Windows.h>
#pragma warning(disable:4996)
int cmp(void *_xp,void *_yp){
	int *xp = (int *)_xp;//������û�д��֪�����ͣ���ֱ��ǿת
	int *yp = (int *)_yp;
	if (*xp > *yp){
		return 1;
	}
	else if (*xp < *yp){
		return -1;
	}
	else{
		return 0;
	}

}

void Swap(void *xp, void *yp,int size){
	char *_xp = (char *)xp;
	char *_yp = (char *)yp;
	char temp;
	for (int i = 0; i < size; i++){
		temp = *_xp;
		*_xp = *_yp;
		*_yp = temp;
		_xp++, _yp++;
	}
}



void Myqsort(void *a, int num, int size, int (*cmp)()){//�ú���ָ�����
	char *xp = (char *)a;
	for (int i = 0; i < num; i++){
		for (int j = 0; j<num - 1 - i; j++){
			if (cmp((xp + size*j), (xp + size*(j + 1)))>0){
				Swap((xp + size*j), (xp + size*(j + 1)), size);
			}
		}
	}
}


int main(){
	int a[] = { 56, 9, 12, 4, -45, 65, 35, -78, 15, 6, 58, -8, 12 };
	int num = sizeof(a) / sizeof(a[0]);
	Myqsort(a, num, sizeof(int), cmp);//������ֱ�Ӵ�������
	for (int i = 0; i < num; i++){
		printf("%d ", a[i]);
	}
	printf("\n");

	system("pause");
	return 0;
}