/*
 * que_tool.c
 *
 *  Created on: 2024/06/02
 *      Author: x-lib
 *
 *
 *      https://daeudaeu.com/stack_queue/
 *      �̋L�����Q��
 *
 *
 */



#if defined(MY_TEST) || defined(SAMPLE_ORIGINAL)
#include <stdio.h>
#endif
#include <string.h>
#include <queue_tool.h>

/* �L���[������������֐� */
void initQueue(QUEUE_T *queue){

    /* �L���[����ɐݒ� */
    queue->head = 0;
    queue->tail = -1;
    memset(queue->data,'0',sizeof(queue->data));
}

/* ENQUEUE����֐� */
void enqueue(QUEUE_T *queue, char input){

    /* �L���[�����t�Ȃ牽�������֐��I�� */
    if((queue->tail + 2) % MAX_NUM == queue->head){
        //printf("�L���[�����t��ENQUEUE�ł��܂���\n");
        return;
    }

    /* �f�[�^���f�[�^�̍Ō���̂P���Ɋi�[ */
    queue->data[(queue->tail + 1) % MAX_NUM] = input;

    /* �f�[�^�̍Ō�����P���Ɉړ� */
    queue->tail = (queue->tail + 1) % MAX_NUM;
}

/* DEQUEUE����֐� */
int dequeue(QUEUE_T *queue){
    int ret = 0;

    /* �L���[����Ȃ牽�������Ɋ֐��I�� */
    if((queue->tail + 1) % MAX_NUM == queue->head){
        //printf("�L���[����ł�\n");
        return -1;
    }

    /* �f�[�^�̐擪����f�[�^���擾 */
    ret = queue->data[queue->head];

    /* �f�[�^�̐擪���P���ɂ��炷 */
    queue->head = (queue->head + 1) % MAX_NUM;

    /* �擾�����f�[�^��ԋp */
    return ret;
}



/*
 * Queue�̕�������A������ɃR�s�[����
 * 2024/06/02 �iprintQueue�̏Ă������j
 */
int QueueToString(QUEUE_T *queue, char string[])
{
    int i = 0;
    int num;

    /* �L���[���̃f�[�^�̌����v�Z */
    if (queue->tail < queue->head) {
        num = queue->tail + MAX_NUM - queue->head + 1;
    } else {
        num = queue->tail - queue->head + 1;
    }

    for(i = 0; i < num; i++){
        /* �f�[�^�̐擪����num���̃f�[�^��string�Ɋi�[ */
        //printf("%c,", queue->data[(queue->head + i) % MAX_NUM]);
        string[i]= queue->data[(queue->head + i)];
    }
    string[num]='\0';
    return num;


}





#if defined(MY_TEST) || defined(SAMPLE_ORIGINAL)
/*
 * �@https://qiita.com/D-3/items/b98b63c2629496856654
*  �v���v���Z�b�T��������AOR�����̋L�����Q��
*/

/* �L���[�̒��g��\�� */
void printQueue(QUEUE_T *queue){
    int i = 0;
    int num;

    /* �L���[���̃f�[�^�̌����v�Z */
    if (queue->tail < queue->head) {
        num = queue->tail + MAX_NUM - queue->head + 1;
    } else {
        num = queue->tail - queue->head + 1;
    }

    printf("�������L���[�̏o������\���Ă��܂�\n");
    for(i = 0; i < num; i++){
        /* �f�[�^�̐擪����num���̃f�[�^��\�� */
        printf("%c,", queue->data[(queue->head + i) % MAX_NUM]);
    }
    printf("\n");
}
#endif

#ifdef MY_TEST

/*
 * ���쌟�ؗp�̊֐�
*/
#define INITIAL		0
#define INPROGRESS	20
#define	POSTPROCESS	30
#define ERROR		40
#define	END		50

int get_number(void){

	char num;
	int number=0;

	QUEUE_T queue;

	int state=INITIAL;


	while(state!=END){
		num=(int)getchar();
		switch(num){
			case '#':
				/* �L���[�������� */
				initQueue(&queue);
				state=INPROGRESS;
				printf("Que Ready.Input Number(s)\n");
				break;
			case '*':
				printf("�L���[�̒��g��\��\n");
				printQueue(&queue);
				state=END;
				break;;

			case '\n':
			case '\r':
				continue;

			default:
				if(state!=INPROGRESS){
					printf("Que is not ready. Hit # to initialize the que.\n");
					break;
				}else{
				printf("[num=%d]\n",num);
				enqueue(&queue, num);
				state=INPROGRESS;
				break;
				}
    		}
	}

	return number;
}


int main(void){
	get_number();

}
#endif




#ifdef SAMPLE_ORIGINAL

/*
	https://daeudaeu.com/stack_queue/
	�I���W�i���̓���m�F���C���֐�
*/


//
int main(void){

    int m;
    int input;
    int output;
    QUEUE_T queue;

    /* �L���[�������� */
    initQueue(&queue);

    while(1){
        /* ���[�U���烁�j���[��I��ł��炤 */
        printf("���j���[\n 1:ENQUEUE\n 2:DEQUEUE\n 3:�L���[�\��\n ����ȊO�̐���:�I��\n");
        scanf("%d", &m);

        /* �I�΂ꂽ���j���[�ɉ����ď����𕪊� */
        if(m == 1){
            printf("ENQUEUE���鐔�́H�i���̐����̂݉��j");
            scanf("%d", &input);
            if (input < 0) {
                printf("���̒l�͎󂯕t���Ă��܂���!\n");
                continue;
            }
            printf("%d��ENQUEUE���܂�\n", input);
            enqueue(&queue, input);
        }else if(m == 2){
            printf("DEQUEUE���܂�\n");
            output = dequeue(&queue);
            if(output != -1){
                printf("%d��DEQUEUE���܂���\n", output);
            }
        } else if(m == 3){
            printf("�L���[�̒��g��\�����܂�\n");
            printQueue(&queue);
        } else {
            /* 1, 2, 3�ȊO�̏ꍇ��while���𔲂��o�����߂�break�����s */
            printf("�I�����܂�\n");
            break;
        }
    }
    return 0;

}
#endif


