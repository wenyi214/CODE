#include"sort.h"

static int Findpostion(int a[], int num){
	int max = a[0];
	int pos = 0;
	for (int i = 0; i < num; i++){
		if (max > a[i]){
			max = a[i];
			pos = i;
		}
	}
	return pos;
}
static Swap(int *x, int *y){
	int temp = *x;
	*x = *y;
	*y = temp;
	//*x ^= *y;
	//*y ^= *x;
	//*x ^= *y;//����򽻻���ֵ�������ֵ��ͬ���������͵���0�ˡ�
}


void Selectsort(int a[], int num){
	while (num){
		int i = Findpostion(a, num);//�ҵ���������С��λ�ã�
		Swap(&a[num - 1], &a[i]);//�����/Сλ�õ�ֵ�����ں���
		num--;//ȥ�����һλ
	}


}