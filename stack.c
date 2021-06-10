#include"stacknode.h"

Stack Stackinit(){
	Stack stack = (Stack)malloc(sizeof(struct Node));
	stack->next = NULL;
	return stack;
}
static Stack Stackcreat(int x){
	Stack stack = (Stack)malloc(sizeof(struct Node));
	stack->data = x;
	stack->next = NULL;
	return stack;
}

void Stackpush(Stack stack, int x){
	assert(stack);
	Stack ass = Stackcreat(x);
	ass->next = stack->next;
	stack->next = ass;

}
Elementtype Stackpop(Stack stack){
	assert(stack);
	if (stack->next == NULL){
		printf("empty\n");
	}
	Stack temp = stack->next;
	stack->next = temp->next;
	Elementtype ass = temp->data;
	free(temp);
	return ass;
}
void Stackprint(Stack stack){
	stack = stack->next;
	if (stack){
		while (stack){
			printf("%d ", stack->data);
			stack = stack->next;
		}
		printf("\n");
	}
	else{
		printf("Empty\n");
	}


}