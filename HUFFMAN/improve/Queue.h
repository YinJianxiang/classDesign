/*************************************************************************
	> File Name: Queue.h
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年12月26日 星期二 08时49分14秒
 ************************************************************************/

 #ifndef _QUEUE_H
 #define _QUEUE_H
 
//---------------------------------------------------------------------------
// 队列状态
//---------------------------------------------------------------------------
 #define Full      -1 
 #define Empty      0 
 #define Normal     1 
 
 #define MAX_SIZE  80
 
 using namespace std;
 
 /*
 *@brief: 队列结构体
 * 
 */
 typedef struct Queue{
     int tag; /*!< 判断队列空满的标志 */ 
     int front; /*!< 队头 */
     int rear;  /*!< 队尾 */ 
     int length; /*!< 队列长度 */
     char elem[MAX_SIZE]; /*!< 队列元素 */
 } Queue;
 
 int InitQueue(Queue * Q);
 int In_Queue(Queue * Q, char x);
 int Out_Queue(Queue * Q, char *x);
 
 #endif
 