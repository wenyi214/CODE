#include<stdio.h>
#include<Windows.h>
#pragma warning (disable:4996)

//������������
int Mystrlen(char a[]){
	int count = 0;
	while (*a){
		count++;
		a++;
	}
	return count;
}


//�ݹ�
//int Mystrlen(char a[]){
//	if (*a=='\0'){
//		return 0;
//	}
//
//	return 1 + Mystrlen(a + 1);//������a++����ʹ�ú�Ӽӣ���ѭ��
//	
//}


//ָ�������ָ����Ԫ�ظ���
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