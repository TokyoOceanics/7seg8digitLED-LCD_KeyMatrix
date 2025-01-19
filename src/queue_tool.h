/*
 * que_tool.h
 *
 *  Created on: 2024/06/02
 *      Author: x-lib
 */

#ifndef QUEUE_TOOL_H_
#define QUEUE_TOOL_H_

#define _H_QUE
/* �Ǘ�����f�[�^�̏����+1 */
#define MAX_NUM (10+1)

/* �L���[�\���� */
typedef struct QUEUE {
    /* �f�[�^�̍Ō�� */
    int tail;
    /* �f�[�^�̐擪 */
    int head;
    /* �L���[����Ă���f�[�^ */
    char data[MAX_NUM];
} QUEUE_T;

void initQueue(QUEUE_T*);
void printQueue(QUEUE_T*);
void enqueue(QUEUE_T*, char);
int dequeue(QUEUE_T*);

int QueueToString(QUEUE_T *, char []);

#endif /* QUEUE_TOOL_H_ */
