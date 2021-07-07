#include"Head.h"

int main(){
	HDdatatype a[] = { 4, 8, 2, 6, 7, 15, 20, 9, 85, 75, 40 };
	int n = sizeof(a) / sizeof(a[0]);
	HD head;
	HeadInit(&head, a, n);
	HeadPrint(&head);
	HeadPush(&head, 13);
	HeadPrint(&head);
	HeadPush(&head, 90);
	HeadPrint(&head);

	HDdatatype max = HeadPop(&head);
	printf("%d\n", max);
	HeadPrint(&head);


	system("pause");
	return 0;
}