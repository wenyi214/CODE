#ifndef __LISTQUEQUE_H__
#define __LISTQUEQUE_H__

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<Windows.h>
#pragma warning(disable:4996)

typedef int Elementtype;
struct Datalist{
	Elementtype data;
	struct Datalist *next;
};

typedef struct Datalist* Dlist;
typedef struct Quelist{
	Dlist front;
	Dlist end;
}Que;

void Quequeinit(Que *list);
void Quelistpush(Que *list, int x);
Elementtype Quelistpop(Que *list);
void Quelistprint(Que *list);
void Quelistdestory(Que *list);



#endif