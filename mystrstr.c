#include<stdio.h>
#include<assert.h>
#include<Windows.h>
#pragma warning(disable:4996)

char *Mystrstr(const char *a, const char *b){
	assert(a);
	assert(b);
	while (*a){
		char *p = (char *)a; //��������ָ�룬��ΪaҪ��������ǵ�λ��
		char *q = (char *)b;//b��Ҫ��֤�Ƚ�ʱ���Ǵ�ͷ��ʼ�Ƚ�
		while (*q&&*p&&(*p == *q)){
			q++; p++;       //�ö���ָ�������λ�ñ仯
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