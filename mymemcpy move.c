#include<stdio.h>
#include<assert.h>
#include<Windows.h>
#pragma warning(disable:4996)
void *Mymemcpy(void *res, const void *sou, size_t num){
	assert(res);
	assert(sou);
	char *x = (char *)res;
	char *y = (char *)sou;
	for (unsigned int i = 0; i < num; i++){
		x[i] = y[i];
	}
	return x;

}

void *Mymemmove(void *res, const void *sou, size_t num){
	assert(res);
	assert(sou);
	//int len = strlen(sou);
	//int lens = strlen(res);
	char *x = (char *)res;
	char *y = (char *)sou;
	if (y < x&&x < (y + num)){
		for (int i = num - 1; i >= 0; i--){//隐式转换，将num转换成有符号了,如果i定义成无符号数就一直循环了
			x[i] = y[i];
		}
	}
	else{
		for (unsigned int i = 0; i < num; i++){
			x[i] = y[i];
		}
	}

	return x;

}


int main(){
	char arr[10] = "12345";
	//char *p = Mymemcpy(arr + 1, arr, strlen(arr) + 1);
	char *p = Mymemmove(arr + 1, arr, strlen(arr) + 1);
	printf("%s\n", p);
	printf("%s\n", arr);
	system("pause");
	return 0;
}