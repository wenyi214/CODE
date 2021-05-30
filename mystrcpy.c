#include<stdio.h>
#include<Windows.h>
#pragma warning(disable:4996)

char *Mystrcpy(char *a, const char *b){
	int i = 0;
	while (b[i]){
		a[i] = b[i];
		i++;
	}
	a[i] = '\0';
	return a;


}

int main(){
	char a[10] = "123";
	char *p = Mystrcpy(a, "abcd");
	printf("%s\n", p);
	printf("%s\n", a);
	system("pause");
	return 0;
}
