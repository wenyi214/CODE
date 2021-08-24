#include<stdio.h>
//A是起始位置
//B是中间位置
//C是终点位置
void Hanuo(int n, char A, char B, char C){
	if (n == 1){
		//只有一个盘子，移动一下，A移动到C
		printf("%c->%c ", A, C);
		return;
	}
	//将A上的n-1个盘子，通过C移动到B上
	Hanuo(n - 1, A, C, B);
	//将A上n-1个盘子移动完之后，将A上的盘子移动到C上
	printf("%c->%c ", A, C);
	//最后将B上的盘子通过A移动到C上
	Hanuo(n - 1, B, A, C);

}

int main(){

	Hanuo(4, 'A', 'B', 'C');
	getchar();
	return 0;
}