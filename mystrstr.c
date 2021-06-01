#include<stdio.h>
#include<assert.h>
#include<Windows.h>
#pragma warning(disable:4996)

char *Mystrstr(const char *a, const char *b){
	assert(a);
	assert(b);
	while (*a){
		char *p = (char *)a; //定义两个指针，因为a要返回相等是的位置
		char *q = (char *)b;//b个要保证比较时都是从头开始比较
		while (*q&&*p&&(*p == *q)){
			q++; p++;       //让定义指针的两个位置变化
		}
		if (*q == '\0'){
			break;
		}
		else{
			a++;
		}

	}
	return a;
}

int main(){
	char a[] = "this is a simple";
	char *p = Mystrstr(a, "simple");
	printf("%s\n", p);
	printf("%s\n", a);

	system("pause");
	return 0;
}