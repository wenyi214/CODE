#pragma once
//数组实现堆栈，top在对尾
#include<stdio.h>
#include<Windows.h>
#include<stdlib.h>
#include<assert.h>
#pragma warning(disable:4996)


typedef int Elementtype;
struct Node{
	int top;
	int capacity;
	Elementtype *data;
};
typedef struct Node* Stack;

Stack Stackinit();//堆栈初始化
void Stackpush(Stack stack, int x);//压栈
Elementtype Stackpop(Stack stack);//出栈，返回出栈的值
void Stackprint(Stack stack);