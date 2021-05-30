#include<stdio.h>
#include<Windows.h>
#pragma warning (disable:4996)

//创建第三变量
int Mystrlen(char a[]){
	int count = 0;
	while (*a){
		count++;
		a++;
	}
	return count;
}


//递归
//int Mystrlen(char a[]){
//	if (*a=='\0'){
//		return 0;
//	}
//
//	return 1 + Mystrlen(a + 1);//不能是a++，先使用后加加，死循环
//	
//}


//指针减法，指针间的元素个数
//int Mystrlen(char a[]){
//	char *p = a;
//	while (*p){
//		p++;
//	}
//	return p - a;
//
//}



int main(){
	char a[] = "asdxcascxc";
	int len = Mystrlen(a);
	printf("%d\n", len);


	system("pause");
	return 0;
}