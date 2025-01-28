/*
 * que_tool.c
 *
 *  Created on: 2024/06/02
 *      Author: x-lib
 *
 *
 *      https://daeudaeu.com/stack_queue/
 *      の記事を参照
 *
 *
 */



#if defined(MY_TEST) || defined(SAMPLE_ORIGINAL)
#include <stdio.h>
#endif
#include <string.h>
#include <queue_tool.h>

/* キューを初期化する関数 */
void initQueue(QUEUE_T *queue){

    /* キューを空に設定 */
    queue->head = 0;
    queue->tail = -1;
    memset(queue->data,'0',sizeof(queue->data));
}

/* ENQUEUEする関数 */
void enqueue(QUEUE_T *queue, char input){

    /* キューが満杯なら何もせず関数終了 */
    if((queue->tail + 2) % MAX_NUM == queue->head){
        //printf("キューが満杯でENQUEUEできません\n");
        return;
    }

    /* データをデータの最後尾の１つ後ろに格納 */
    queue->data[(queue->tail + 1) % MAX_NUM] = input;

    /* データの最後尾を１つ後ろに移動 */
    queue->tail = (queue->tail + 1) % MAX_NUM;
}

/* DEQUEUEする関数 */
int dequeue(QUEUE_T *queue){
    int ret = 0;

    /* キューが空なら何もせずに関数終了 */
    if((queue->tail + 1) % MAX_NUM == queue->head){
        //printf("キューが空です\n");
        return -1;
    }

    /* データの先頭からデータを取得 */
    ret = queue->data[queue->head];

    /* データの先頭を１つ後ろにずらす */
    queue->head = (queue->head + 1) % MAX_NUM;

    /* 取得したデータを返却 */
    return ret;
}



/*
 * Queueの文字列を、文字列にコピーする
 * 2024/06/02 （printQueueの焼き直し）
 */
int QueueToString(QUEUE_T *queue, char string[])
{
    int i = 0;
    int num;

    /* キュー内のデータの個数を計算 */
    if (queue->tail < queue->head) {
        num = queue->tail + MAX_NUM - queue->head + 1;
    } else {
        num = queue->tail - queue->head + 1;
    }

    for(i = 0; i < num; i++){
        /* データの先頭からnum個分のデータをstringに格納 */
        //printf("%c,", queue->data[(queue->head + i) % MAX_NUM]);
        string[i]= queue->data[(queue->head + i)];
    }
    string[num]='\0';
    return num;


}





#if defined(MY_TEST) || defined(SAMPLE_ORIGINAL)
/*
 * 　https://qiita.com/D-3/items/b98b63c2629496856654
*  プリプロセッサ条件分岐、OR条件の記事を参照
*/

/* キューの中身を表示 */
void printQueue(QUEUE_T *queue){
    int i = 0;
    int num;

    /* キュー内のデータの個数を計算 */
    if (queue->tail < queue->head) {
        num = queue->tail + MAX_NUM - queue->head + 1;
    } else {
        num = queue->tail - queue->head + 1;
    }

    printf("左側がキューの出口側を表しています\n");
    for(i = 0; i < num; i++){
        /* データの先頭からnum個分のデータを表示 */
        printf("%c,", queue->data[(queue->head + i) % MAX_NUM]);
    }
    printf("\n");
}
#endif

#ifdef MY_TEST

/*
 * 動作検証用の関数
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
				/* キューを初期化 */
				initQueue(&queue);
				state=INPROGRESS;
				printf("Que Ready.Input Number(s)\n");
				break;
			case '*':
				printf("キューの中身を表示\n");
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
	オリジナルの動作確認メイン関数
*/


//
int main(void){

    int m;
    int input;
    int output;
    QUEUE_T queue;

    /* キューを初期化 */
    initQueue(&queue);

    while(1){
        /* ユーザからメニューを選んでもらう */
        printf("メニュー\n 1:ENQUEUE\n 2:DEQUEUE\n 3:キュー表示\n それ以外の数字:終了\n");
        scanf("%d", &m);

        /* 選ばれたメニューに応じて処理を分岐 */
        if(m == 1){
            printf("ENQUEUEする数は？（正の整数のみ化）");
            scanf("%d", &input);
            if (input < 0) {
                printf("負の値は受け付けていません!\n");
                continue;
            }
            printf("%dをENQUEUEします\n", input);
            enqueue(&queue, input);
        }else if(m == 2){
            printf("DEQUEUEします\n");
            output = dequeue(&queue);
            if(output != -1){
                printf("%dをDEQUEUEしました\n", output);
            }
        } else if(m == 3){
            printf("キューの中身を表示します\n");
            printQueue(&queue);
        } else {
            /* 1, 2, 3以外の場合はwhile文を抜け出すためにbreakを実行 */
            printf("終了します\n");
            break;
        }
    }
    return 0;

}
#endif


