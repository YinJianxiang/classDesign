/*************************************************************************
	> File Name: Queue.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年12月26日 星期二 08时38分34秒
 ************************************************************************/

#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>

#include"Queue.h"

int InitQueue(Queue * Q) {
	if (!Q)
		return 1;
	Q->tag = Empty;
	Q->front = Q->rear = 0;
	Q->len = 0;

	return 0;
}

int In_Queue(Queue * Q, char x) {
	if (Q->front == Q->rear && Q->tag == Normal)
		return Full;	

	Q->elem[Q->rear] = x;
	Q->rear = (Q->rear + 1) % MAX_SIZE;
	Q->len++;
	Q->tag = Normal;
	return Normal;
}

int Out_Queue(Queue * Q, char *x) {
	if (Q->tag == Empty)
		return Empty;

	*x = Q->elem[Q->front];
	Q->len--;
	Q->front = (Q->front + 1) % MAX_SIZE;

	if (Q->front == Q->rear)
		Q->tag = Empty;

	return Normal;
}
