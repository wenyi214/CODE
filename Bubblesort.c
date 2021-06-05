#include"sort.h"
static Swap(int *x, int *y){
	*x ^= *y;
	*y ^= *x;
	*x ^= *y;
}


void Bubblesort(int a[], int num){
	for (int i = 0; i < num; i++){
		int flag = 0;
		for (int j = i; j < num; j++){
			if (a[i]>a[j]){
				Swap(&a[i], &a[j]);
				flag = 1;
			}

		}
		if (flag == 0){
			break;
		}
	}
}