#include<stdio.h>
//A����ʼλ��
//B���м�λ��
//C���յ�λ��
void Hanuo(int n, char A, char B, char C){
	if (n == 1){
		//ֻ��һ�����ӣ��ƶ�һ�£�A�ƶ���C
		printf("%c->%c ", A, C);
		return;
	}
	//��A�ϵ�n-1�����ӣ�ͨ��C�ƶ���B��
	Hanuo(n - 1, A, C, B);
	//��A��n-1�������ƶ���֮�󣬽�A�ϵ������ƶ���C��
	printf("%c->%c ", A, C);
	//���B�ϵ�����ͨ��A�ƶ���C��
	Hanuo(n - 1, B, A, C);

}

int main(){

	Hanuo(4, 'A', 'B', 'C');
	getchar();
	return 0;
}