/*
 * 	2桁の7セグに、偶数は桁1、奇数は桁２に、それぞれ交互に数値を表示する。
 * 	SysTick_Handlerのつづりを正確にすること。
 */

/*
	ボードのLEDとスイッチの接続
 	A552-SRD（2桁の7セグLED表示器）
 	SegA	PA4
 	SegB	PA5
 	segC	PA6
 	SegD	PA7
 	SegE	PA8
 	SegF	PA9
 	SegG	PA10
 	SEGDP	PA11

 	SelDig1	PB5
 	SelDig2	PB6

	押しボタンスイッチ
 	SW1		PC0
 	SW2		PC1

 */



//#include <stddef.h>
#include "stm32f10x.h"

//７segLEDの表示データ、負論理
int seg7[]={0xc0, 0xf9, 0xa4,0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90};

//7segLEDの表示桁選択、負論理
//             76543210
int digit[]={0b11011111,0b10111111};


void init_system(void);


void my_delay(int n);//ループ処理による待ち時間生成関数
void Wait_Time(int);//SysTick_Handlerを使う待ち時間生成関数


int TimeInt=0;	//Waitする間隔を指定するためのグローバル変数（SysTickで減算）

#define Delay_Time	10	//Wait時間　1ms単位


int main(void)
{

	init_system();


//	while(1){
//		for(int n=0; n<10; n++)
//			for(int i=n; i<n+2; i++){
//				GPIOB->ODR=digit[i%2];	//7segの表示桁を切り替える
//				GPIOA->ODR=seg7[i]<<4;	//7segに数字を表示
//				Wait_Time(Delay_Time);
//			}
//	}

	while(1){
			for(int i=0; i<2; i++){
				GPIOB->ODR=digit[i%2];	//7segの表示桁を切り替える
				GPIOA->ODR=seg7[i]<<4;	//7segに数字を表示
				Wait_Time(Delay_Time);
			}
		}


}



void init_system(void)
{
	/* クロック情報取得／設定の為の構造体定義 */
	RCC_ClocksTypeDef RCC_Clocks;


	/* システムのクロック情報の取得 */
	RCC_GetClocksFreq(&RCC_Clocks);

	/* SysTick タイマの設定 */
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);	// SysTick タイマ割込みの間隔を 1ms に設定



	RCC->APB2ENR = RCC_APB2ENR_IOPAEN |	// GPIOA のクロックを有効にする
				   RCC_APB2ENR_IOPBEN |	// GPIOB のクロックを有効にする
				   RCC_APB2ENR_IOPCEN;	// GPIOC のクロックを有効にする


//               FEDCBA98
	GPIOA->CRH=0x44441111;
//               76543210
	GPIOA->CRL=0x11114444;

//               76543210
	GPIOB->CRL=0x41144444;

//               76543210
	GPIOC->CRL=0x44444444;
}







void SysTick_Handler(void)
{
	//割り込み間隔１ｍｓ、1回のダウンカウントに１ｍｓ
	if(TimeInt>0) TimeInt--;


}



