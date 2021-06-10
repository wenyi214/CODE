#include"stacklist.h"

int main(){
	Stack stack = Stackinit();
	Stackpush(stack, 1);
	Stackpush(stack, 2);
	Stackpush(stack, 3);
	Stackprint(stack);
	Elementtype result = Stackpop(stack);
	Stackprint(stack);
	printf("%d\n", result);
	system("pause");
	return 0;
}