#include "stm32f10x.h"
#include <stdlib.h>

#define C4 523 //ど
#define D4 587 //れ
#define E4 659 //み
#define F4 698 //ふぁ
#define G4 783 //そ
#define A4 880 //ら
#define B4 987 //し
#define C5 1046 //ど
#define D5 1174 //れ

//スイッチの値を取得するためのマスク
#define SW0 0x1
#define SW1 0x2

#define SW0_On 1
#define SW1_On 0

void setup_pwm(void);
void play_tone(unsigned int frequency);
void Wait_Time(int count);
void stop_tone(void);
unsigned int Get_SW(void);
void Seg7_Out_Data(unsigned int);


int main() {
    int delay = 2500000;


    unsigned int R4 ;//7セグ点滅用
    unsigned int sw;//読み込んだスイッチの値を格納する変数
    int state = 0;//スイッチの状態を保存


    setup_pwm();

    unsigned int melody[] =  {B4, G4, D4, G4, A4, D5
    							, D4, A4, B4, A4, D4, G4};
    int i =0;


    while (1){

    	sw = Get_SW();

    	if(sw & SW0){
    		state = SW0_On;
    	}else if(sw & SW1){
    		state = SW1_On;
    	}

    	if(state==0){
    		if(i==12)i=0;
    		if(i==5|i==11){
    			play_tone(melody[i]);
    			Wait_Time(delay*2);
    		}else{
    	   		play_tone(melody[i]);
    			Wait_Time(delay);
    		}

    		i++;


        // 乱数を生成し、それを7セグLEDに出力
    	R4 = ~0x0010;
    	R4 = R4 << (rand() % 8);
    	Seg7_Out_Data(R4);


    	}else{
    		stop_tone();
    	}


    }
}

unsigned int Get_SW(void){
	unsigned int sw;
	sw = GPIOC->IDR;
	sw =~sw;
	sw = sw & (SW0|SW1);

	return sw;
}


void Wait_Time(int count){
        for(int i = 0; i<count; i++){}
}

void setup_pwm() {
    // GPIOBとTIM3のクロックを有効にする
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN|RCC_APB2ENR_IOPBEN|RCC_APB2ENR_IOPCEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // PBAを設定する（7セグ用）
    GPIOA->CRL =0x11114444;
    GPIOA->CRH =0x44441111;

    // PB0をに設定する
    GPIOB->CRL = 0x4444444B;

    // PWMモードを選択し、PWM信号をピンに出力する
    TIM3->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2;
    TIM3->CCER |= TIM_CCER_CC3E;

    // プリスケーラを設定する
    TIM3->PSC = 720-1;

    // タイマを有効にするコントロールレジスタ。　タイマのON・OFFなど設定
    TIM3->CR1 |= TIM_CR1_CEN;

    //自動リロードレジスタを設定する(カウント上限回数を設定する)
    //TIM3->ARR = 1000-1;
}

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

void Seg7_Out_Data(unsigned int data){
	GPIOA -> ODR =data & 0x7f0;
	//7セグの接続ポート以外のビットをマスクする値
}


/*
PB0はTIM3のチャネル3

CCMR2
タイマーの動作モードを制御するレジスタらしい PWMモードを選択するために使用

CCER
出力を有効にするためのレジスタらしい PWM信号をピンに出力するために必要

CCR3
PWMのデューティサイクルを制御するレジスタ　デューティサイクルとは、1つのPWMサイクルの中で電圧がHIGHである時間の割合

PWM信号の周波数とデューティサイクルを制御し、結果的にブザーの音を制御

PWMの周波数は、タイマーのカウントアップ周波数（システムクロック周波数をプリスケーラーで割った値）と、タイマーのカウントアップが一巡するまでの値（自動リロード値）によって決まる。

システムクロックが72MHz、プリスケーラーが720、自動リロード値が1000の場合、PWMの周波数は↓

PWM周波数 = システムクロック周波数 / プリスケーラー / 自動リロード値
          = 72MHz / 720 / 1000
          = 100Hz
PWMの周波数を決定するためには
          自動リロード値 = システムクロック周波数 / プリスケーラー / PWM周波数
*/
