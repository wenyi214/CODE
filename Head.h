#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<Windows.h>
#include<assert.h>

typedef int HDdatatype;
typedef struct Head{
	HDdatatype *a;
	int size;
	int capacity;

}HD;

void HeadInit(HD *head, HDdatatype b[], int n);//堆初始化
void HeadDestroy(HD *head);//释放堆

void HeadPush(HD *head, HDdatatype x);//插入元素并保持堆的性质
HDdatatype HeadPop(HD *head);//弹出堆的首元素(优先级最高的）
HDdatatype HeadTop(HD *head);//得到堆的首元素

bool HeadIsEmpty(HD *head);//判断堆是否为空
bool HeadIsFull(HD *head);//判断堆是否满

int HeadSize(HD *head);//得到堆元素长度

void HeadPrint(HD *head);//打印堆元素
