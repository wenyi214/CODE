#pragma once
//��ͷ�ڵ�
//����ʵ�ֶ�ջ��ͷ�ڵ�ΪTOP������ɾ�������룬β�ڵ㲻����ɾ��
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