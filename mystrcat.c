#include<stdio.h>
#include<Windows.h>
#pragma warning(disable:4996)

char *Mystrcat(char *a, const char *b){
	char *res = a;
	while (*a){
		a++;
	}
	while (*b){
		*a = *b;
		a++, b++;
	}
	*a = '\0';
	return res;
}

int main(){
	char a[20] = "abcd";
	char *p = Mystrcat(a, "1234");
	printf("%s\n", p);
	printf("%s\n", a);

	system("pause");
	return 0;
}