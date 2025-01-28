/*
 * buzzer.c
 *
 *  Created on: 2024/06/03
 *      Author: x-lib
 */

/*
 * PB0 Buzzer
 *
 *

 * PB0の初期化のメモ まじないとして。

  	//TIM3 有効化して、PB0をTIM3とする
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;	//TIM3の有効化


  	//ポートBの入出力設定　GPIOB->CRL設定は、ポートBの入出力設定　pb0をTIM3とする
	//PB0　CNF=10、MODE=11は　オルタネート出力・プッシュプル
	//			 76543210
	GPIOB->CRL=0x???????B;

    // PWMモードを選択し、PWM信号をピンに出力する
    TIM3->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2;
    TIM3->CCER |= TIM_CCER_CC3E;

    // プリスケーラを設定する
    TIM3->PSC = 720-1;

    // タイマを有効にするコントロールレジスタ。　タイマのON・OFFなど設定
    TIM3->CR1 |= TIM_CR1_CEN;
 *
 *
 *使い方
 	melody_address=melody[1];　//楽曲データの先頭アドレス
  	play_music(melody_address);//演奏



 2024/06/05
 keypad_clac_sound関数、inProgress_sount関数において、
 WaitTimeによるディレイ（SysTick使用）を止めたところ、意図する動作となった。

 *
 */
#include <stm32f10x.h>
#include "buzzer.h"

//extern const Note melody[][20];

void play_tone(unsigned int frequency) {
    // 自動リロードレジスタと比較レジスタを周波数に応じて設定する
    TIM3->ARR = (72000000 / frequency / 720) - 1;
    TIM3->EGR = TIM_EGR_UG;  // 追加: レジスタの変更をすぐに反映する
    TIM3->CCR3 = (TIM3->ARR + 1) / 2;  // 50% デューティサイクル
}

void stop_tone() {
    // PWM信号を停止する
    TIM3->CCR3 = 0;
}




void play_music(Note data[]){
	int i=0;
	while(data[i].pitch!=0){
		play_tone(data[i].pitch);
		Wait_Time(data[i].duration);
		i++;
		//time_data=i;
		stop_tone();
	}

}


void keypad_clack_sound(void){

	//キーボード打鍵音

//	Note mldy[]={{C4,1000},{D4,10},{0,0}};
//	play_music(mldy);

	play_tone(C4);
	delay_us(10000);
	stop_tone();
	play_tone(C5);
	delay_us(10000);
	stop_tone();

}

void inProgress_sound(void){

	play_tone(D5);
	delay_us(10000);
	stop_tone();
	play_tone(C4);
	delay_us(100);
	stop_tone();

}


