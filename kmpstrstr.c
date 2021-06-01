#include<stdio.h>
#include<Windows.h>
#pragma warning(disable:4996)

void Createnext(int *next,const char *b){
	int len = strlen(b);
	int j = 0;
	next[0] = 0;
	for (int i = 1; i < len; i++){
		while (b[i] != b[j] && j >= 1){//���жϲ���ȵ����
			j = next[j - 1];//��j����ǰ��
		}
		if (b[i] == b[j]){
			j++;
		}
		next[i] = j;
	}
}

char *Mystrstr(const char *a, const char *b){
	char *res = (char *)a;
	int len = strlen(b);
	int *next = (int *)malloc(sizeof(int)*len);
	Createnext(next, b);
	int j = 0;
	for (unsigned int i = 0; i < strlen(a); i++){
		while (a[i] != b[j]&&j>0){//���жϲ���ȵ����
			j = next[j - 1];//��j����ǰ�棬next�ǰ���b��õ�
		}
		if (a[i] == b[j]){//����a[i]==b[i],     b��С��aС
			j++;
		}
		if (j == len){
			free(next);
			return res + i - j + 1;
		}
	}
	free(next);
	return NULL;
}

int main(){
	char a[] = "this is a simple";
	char *p = Mystrstr(a, "simple");
	printf("%s\n", p);
	system("pause");
	return 0;
}