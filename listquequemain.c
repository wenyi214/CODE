#include"listqueque.h"

int main(){
	Que s;
	Quequeinit(&s);
	Quelistpush(&s, 3);
	Quelistpush(&s, 2);
	Quelistprint(&s);
	Quelistpop(&s);
	Quelistprint(&s);
	Quelistdestory(&s);
	system("pause");
	return 0;
}