#pragma once
//����ʵ�ֶ�ջ��top�ڶ�β
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

Stack Stackinit();//��ջ��ʼ��
void Stackpush(Stack stack, int x);//ѹջ
Elementtype Stackpop(Stack stack);//��ջ�����س�ջ��ֵ
void Stackprint(Stack stack);