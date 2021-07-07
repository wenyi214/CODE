#include"Head.h"

static void Swap(int *px, int *py){
	int temp = *px;
	*px = *py;
	*py = temp;
}
//从下往上调整。得到大根堆
static void AdjustDown(HDdatatype a[], int size, int parent){
	int child = parent * 2 + 1;//先假设左边的数比根数大
	while (child < size){//没有孩子结点退出
		if (child + 1 < size&&a[child] > a[child + 1]){//如果右边的数大，child++就得到右边数的下标
			child++;
		}
		if (a[child] < a[parent]){//如果孩子结点比父节点大就就交换
			Swap(&a[child], &a[parent]);
			parent = child;
			child = parent * 2 + 1;
		}
		else{//直到不大就退出
			break;
		}
	}
}
static void AdjustUp(HDdatatype a[], int child){//向上调整
	while (child > 0){//孩子节点等于0，没有父节点退出
		int parent = (child - 1) / 2;//得到父节点
		if (a[child]<a[parent]){//孩子节点比父节点大交换
			Swap(&a[child], &a[parent]);
			child = parent;
		}
		else{//直到不大退出
			break;
		}
	}
}

//将数组b[]，调整成堆，
void HeadInit(HD *head, HDdatatype b[],int n){
	head->a = (HDdatatype *)malloc(sizeof(HDdatatype)*n);
	if (head->a == NULL){
		printf("malloc fail!\n");
		exit(-1);
	}
	for (int i = 0; i < n; i++){
		head->a[i] = b[i];
	}
	head->size = n;
	head->capacity = n;

	for (int i=(head->size-1-1)/2;i>=0;i--)
	{
		AdjustDown(head->a, head->size, i);
	}
}

void HeadDestroy(HD *head){//释放
	free(head->a);
	head->a = NULL;
	head->capacity = 0;
	head->size = 0;
}
//在最后一个位置插入，再向上调整成堆
void HeadPush(HD *head, HDdatatype x){
	assert(head);
	if (HeadIsFull(head)){
		HDdatatype *temp = (HDdatatype *)realloc(head->a, sizeof(HDdatatype)*head->capacity * 2);
		if (temp == NULL){
			printf("realloc fail\n");
			HeadDestroy(head);
			exit(-1);
		}
		head->a = temp;
		head->capacity *= 2;
	}
	head->a[head->size] = x;
	head->size++;
	AdjustUp(head->a, head->size - 1);

}

//假设有N各元素，将第一个元素(0)与最后一个元(N-1)素交换，再将前面N-1个元素向下调整成堆
HDdatatype HeadPop(HD *head){
	assert(head);
	int max = head->a[0];
	Swap(&head->a[0], &head->a[head->size - 1]);
	head->size--;
	AdjustDown(head->a, head->size - 1, 0);
	return max;
}

HDdatatype HeadTop(HD *head){
	assert(head);
	return head->a[0];
}

bool HeadIsEmpty(HD *head){
	if (head->size == 0){
		return true;
	}
	return false;
}

bool HeadIsFull(HD *head){
	if (head->size == head->capacity){
		return true;
	}
	return false;
}

int HeadSize(HD *head){
	assert(head);
	return head->size;
}

void HeadPrint(HD *head){
	for (int i = 0; i < head->size; i++){
		printf("%d ", head->a[i]);
	}
	printf("\n");
}