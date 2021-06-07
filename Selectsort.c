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
	//*x ^= *y;//用异或交换两值，入股两值相同，异或出来就等于0了。
}


void Selectsort(int a[], int num){
	while (num){
		int i = Findpostion(a, num);//找到最大或者最小的位置，
		Swap(&a[num - 1], &a[i]);//将最大/小位置的值，放在后面
		num--;//去除最后一位
	}


}