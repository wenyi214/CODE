#include"stacklist.h"

Stack Stackinit(){
	Stack stack = (Stack)malloc(sizeof(struct Node));
	stack->data = (Elementtype *)malloc(sizeof(Elementtype)* 4);
	stack->capacity = 4;
	stack->top = -1;
	return stack;
}

void Stackpush(Stack stack, int x){
	assert(stack);
	if (stack->top == stack->capacity){//ÂúÁË¼Ó¿Õ¼ä
		stack->data = realloc(stack->data, sizeof(Elementtype)*stack->capacity * 2);
		stack->capacity *= 2;
	}
	stack->top++;
	stack->data[stack->top] = x;

}
Elementtype Stackpop(Stack stack){
	assert(stack);
	if (stack->top == -1){
		printf("empty!\n");
	}
	Elementtype ass = stack->data[stack->top];
	stack->top--;
	return ass;


}
void Stackprint(Stack stack){
	for (int i = 0; i < stack->top + 1; i++){
		printf("%d ", stack->data[i]);
	}
	printf("\n");

}