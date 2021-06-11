#pragma once
#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include<assert.h>
#pragma warning(disable:4996)

#define MAX 4;

typedef int Elementtype;

typedef struct Quelist{
	Elementtype *a;
	int front;
	int end;
	int capacity;
}Que;

void Quequeinit(Que *list);
void Quelistpush(Que *list, int x);
Elementtype Quelistpop(Que *list);
void Quelistprint(Que *list);