/*
 * que_tool.h
 *
 *  Created on: 2024/06/02
 *      Author: x-lib
 */

#ifndef QUEUE_TOOL_H_
#define QUEUE_TOOL_H_

#define _H_QUE
/* 管理するデータの上限個数+1 */
#define MAX_NUM (10+1)

/* キュー構造体 */
typedef struct QUEUE {
    /* データの最後尾 */
    int tail;
    /* データの先頭 */
    int head;
    /* キューされているデータ */
    char data[MAX_NUM];
} QUEUE_T;

void initQueue(QUEUE_T*);
void printQueue(QUEUE_T*);
void enqueue(QUEUE_T*, char);
int dequeue(QUEUE_T*);

int QueueToString(QUEUE_T *, char []);

#endif /* QUEUE_TOOL_H_ */
