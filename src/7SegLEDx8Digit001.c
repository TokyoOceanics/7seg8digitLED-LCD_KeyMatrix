/*
 * 7SegLEDx8Digit001.c
 *
 *  Created on: 2024/03/04
 *      Author: shidou01
 *
 *      河原先生の表示回路中継基板の動作確認　7セグLED×8桁部分
 *
 */


/*
 * SegDP~SegA	PB15~PB8
 * Dgt7~Dgt0	PC7~PC0
 * Buzzer		PB0
 * UserButtonSW	PC13
 *
 * *
 *SegDP	PB15	CN10-26
 *SegG	PB14	CN10-28
 *SegF	PB13	CN10-30
 *SegE	PB12	CN10-16
 *SegD	PB11	CN10-18
 *SegC	PB10	CN10-25
 *SegB	PB09	CN10-05
 *SegA	PB08	CN10-03
 *
 *Dgt7	PC07	CN10-19
 *Dgt6	PC06	CN10-04
 *Dgt5	PC05	CN10-06
 *Dgt4	PC04	CN10-34
 *Dgt3	PC03	CN07-37
 *Dgt2	PC02	CN07-35
 *Dgt1	PC01	CN07-36
 *Dgt0	PC00	CN07-38
 *
 *Buz	PB0		CN07-34
 *
 */


#include "main.h"

#define UserButtonSW	(GPIOC->IDR)&(0x01<<13)



//グローバル変数
int TimeInt=0;	//delay処理（Waitする間隔を指定するため）のグローバル変数（SysTickで減算）




//関数のプロトタイプ宣言
void my_init_system(void);
void Wait_Time(int time);	//SysTick_Handlerを使う待ち時間処理

void SelectDigit(int digit);
void Display_7Seg(int number);



//
//メイン関数
//
int main(void)
{

	my_init_system();//ハードウェアの初期化

	while(1){
		for(int digit=0; digit<=7; digit++){
			SelectDigit(digit);
			for(int data=0; data<10; data++){
				Display_7Seg(data);
				Wait_Time(50);
			}

		}

	}
}




//
//ハードウェアの初期化
//

void my_init_system(void)
{

	/* クロック情報取得／設定の為の構造体定義 */
	RCC_ClocksTypeDef RCC_Clocks;

	/* システムのクロック情報の取得 */
	RCC_GetClocksFreq(&RCC_Clocks);

	/* SysTick タイマの設定 */
	/*SysTick タイマ割込みの間隔を 1ms に設定 */
	// 割り込み間隔はSysTick_Config()の引数をRCC_Clocks.HCLK_Frequency / 1000　として、1msec
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);	//

	//GPIO　AとBを有効化
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN|RCC_APB2ENR_IOPCEN;	// GPIOB,Cのクロックを有効にする

	//TIM3 有効化して、PB0をTIM3とする
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;	//TIM3の有効化

	//ポートBの入出力設定　GPIOB->CRL設定は、ポートBの入出力設定　pb0をTIM3とする
	//PB0　CNF=10、MODE=11は　オルタネート出力・プッシュプル
	//			 76543210
	GPIOB->CRL=0x4444444B;
	//			 FEDCBA98
	GPIOB->CRH=0x11111111;

	//ポートC
	//			 76543210
	GPIOC->CRL=0x11111111;
	//			 FEDCBA98
	GPIOC->CRH=0x11141111;



}




void SelectDigit(int digit)
{

	if( (0<=digit) && (digit<=7) )
		GPIOC->ODR=(0x01<<digit);
	else
		GPIOC->ODR=0x00;

}



void Display_7Seg(int number)
{
/*
**      < 7 セグメント LED のポート割り当て >
**
**             a		a - PB8
**           ----		b - PB9
**        f / g / b		c - PB10
**          ----		d - PB11
**       e /   / c		e - PB12
**         ----			f - PB13
**          d			g - PB14
*/

/* 7 セグメント LED 表示パターン用のデータ配列（正論理として定義） */

	static int Seg7led_Table[] = { 0x3f00,	// 0
								   0x0600,	// 1
								   0x5b00,	// 2
								   0x4f00,	// 3
								   0x6600,	// 4
								   0x6d00,	// 5
								   0x7d00,	// 6
								   0x2700,	// 7
								   0x7f00,	// 8
								   0x6f00	// 9
	};

	if ((number >= 0) && (number <= 9)) {				// 数値が 0 ～ 9 の範囲なら指定された数字のパターンを
		GPIOB->ODR = Seg7led_Table[number] & 0x7f00;	// 7 セグメント LED に表示する（負論理の為ビット反転）
	} else {											// 0 ～ 9 の範囲外の値が指定された場合は
		GPIOB->ODR = 0x7f00;							// ７セグメント LED を全消灯する
	}
}






/*
 * 待ち時間関数　（SysTick_Handlerと連携）
 */

void Wait_Time(int time)
{
	TimeInt=time;
	while(TimeInt>0);	//カウンタの値が0になるまで待つ。カウンタはSysTick_Handlerで減算されている。


}





/*
 * SysTick_Handler関数
*/

void SysTick_Handler(void)
{





	//
	//待ち時間関数　Wait_Time()用の変数の処理
	//
	if(TimeInt>0) TimeInt--;





}
