#pragma once
//带头节点
//链表实现堆栈，头节点为TOP，方便删除，插入，尾节点不方便删除
#include<stdio.h>
#include<Windows.h>
#include<assert.h>
#include<stdlib.h>
#pragma warning(disable:4996)

typedef int Elementtype;
struct Node {
	Elementtype data;
	struct Node *next;
};
typedef struct Node* Stack;

Stack Stackinit();
void Stackpush(Stack stack,int x);
Elementtype Stackpop(Stack stack);
void Stackprint(Stack stack);