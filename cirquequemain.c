#include"cirqueque.h"


int main(){
	Que s;
	Quequeinit(&s);
	Quelistpush(&s, 3);
	Quelistpush(&s, 2);
	Quelistpush(&s, 1);
	Quelistprint(&s);
	Elementtype result = Quelistpop(&s);
	printf("%d\n", result);
	Quelistprint(&s);

	system("pause");
	return 0;
}