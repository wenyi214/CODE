#include"sort.h"

void Print(int a[], int num){
	for (int i = 0; i < num; i++){
		printf("%d ", a[i]);
	}
	printf("\n");
}

int main(){
	int a[] = { 4, 8, 3, 9, 7, 1 };
	int num = sizeof(a) / sizeof(a[0]);
	//Bubblesort(a, num);
	//Quicksort(a, 0, num - 1);
	//Isertsort(a, num);
	Selectsort(a, num);
	Print(a, num);
	system("pause");
	return 0; 
}