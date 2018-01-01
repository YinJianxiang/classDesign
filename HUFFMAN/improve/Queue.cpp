/*************************************************************************
	> File Name: Queue.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年12月26日 星期二 08时38分34秒
 ************************************************************************/
/*
*@file Queue.cpp 
*@biref 关于队列函数的实现 
*/

 #include<iostream>
 #include<cstdio>
 #include<cstdlib>
 #include<cstring>
 
 #include"Queue.h"
 
/** 初始化队列，返回队列的状态
*@prama Q {Queue *}队列头指针
*
*@return {int} 队列状态
*/
 int InitQueue(Queue * Q) {
	 if (!Q)
		 return 1;
	 Q->tag = Empty;
	 Q->front = Q->rear = 0;
	 Q->length = 0;
 
	 return 0;
 }
 
/** 向队列中插入元素，返回队列的状态
*@prama Q {Queue *}队列头指针
*@prama x {const char}进入队列的元素
*
*@return {int} 队列状态
*/
 int In_Queue(Queue * Q,char x) {
	 if (Q->front == Q->rear && Q->tag == Normal)
		 return Full;	
 
	 Q->elem[Q->rear] = x;
	 Q->rear = (Q->rear + 1) % MAX_SIZE;
	 Q->length++;
	 Q->tag = Normal;
	 return Normal;
 }
 

 /** 从队列中取入元素，返回队列的状态
*@prama Q {Queue *}队列头指针
*@prama x {char *}从队列中取出元素的指针
*
*@return {int} 队列状态
*/
 int Out_Queue(Queue * Q, char *x) {
	 if (Q->tag == Empty)
		 return Empty;
 
	 *x = Q->elem[Q->front];
	 Q->length--;
	 Q->front = (Q->front + 1) % MAX_SIZE;
 
	 if (Q->front == Q->rear)
		 Q->tag = Empty;
 
	 return Normal;
 }
 