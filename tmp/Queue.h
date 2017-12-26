/*************************************************************************
	> File Name: Queue.h
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年12月26日 星期二 08时49分14秒
 ************************************************************************/

#ifndef _QUEUE_H
#define _QUEUE_H


#define FULL      -1
#define EMPTY      0
#define NORMAL     1

#define MAX_SIZE  80

using namespace std;

typedef struct Queue{
    int tag;
    int front;
    int rear;
    int len;
    char elem[MAX_SIZE];
} Queue;

int InitQueue(Queue * Q);
int In_Queue(Queue * Q, char x);
int Out_Queue(Queue * Q, char *x);

#endif
