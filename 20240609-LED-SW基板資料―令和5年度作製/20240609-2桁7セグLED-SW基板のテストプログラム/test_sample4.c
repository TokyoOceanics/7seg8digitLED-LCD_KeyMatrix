/*
 * 	00からFFまで数値を表示する。
 * 	ブランク時間を設けて、表示を見やすくする。
 *
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

int seven_segment_led[3];


//7segLEDの表示桁選択、負論理
//             76543210   76543210   76543210
int digit[]={0b11011111,0b10111111,0b11111111};
//           1桁目の表示    2桁目の表示   全消灯


void my_init_system(void);


void my_delay(int n);//ループ処理による待ち時間生成関数
void Wait_Time(int);//SysTick_Handlerを使う待ち時間生成関数


int TimeInt=0;	//Waitする間隔を指定するためのグローバル変数（SysTickで減算）

#define Delay_Time	10	//Wait時間　1ms単位


int set_segment(int value);
int num_to_2digit(int num,int *digit_one, int *digit_ten);


int main(void)
{

	my_init_system();



	while(1){
		for(int i=0; i<100; i++){
			set_segment(i);
			Wait_Time(1000);
		}
	}

}



void my_init_system(void)
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


//2桁の数値から1の位、10の位ごとの数値を求める。
int num_to_2digit(int num,int *digit_one, int *digit_ten)
{
	//値が負の場合と、100を超える場合はエラーとする。
	if((num>100)||(num<0)){
		*digit_one=0;
		*digit_ten=0;
		return -1;
	}

	//0～99なら正常動作
	*digit_one=num%10;
	*digit_ten=num/10;

	return 0;	//正常終了

}

//表示させたい数値を7セグLEDの各桁にセットする。
int set_segment(int value)
{
	int digit_one, digit_ten;

	num_to_2digit( value, &digit_one, &digit_ten);

	seven_segment_led[0]=seg7[digit_one];
	seven_segment_led[1]=seg7[digit_ten];
	seven_segment_led[2]=0xff;

	return 0;
}




void SysTick_Handler(void)
{

	int segment_digit;
	static int blank_count=0;
	
	//割り込み間隔１ｍｓ、1回のダウンカウントに１ｍｓ
	if(TimeInt>0) TimeInt--;

	
	//各桁の表示切替と、表示切替の瞬間にいったん全桁表示OFFのブランクを入れる処理
	//ブランクを入れるのは、少々念を入れすぎかも。
	switch(blank_count){
	
		case 0:
			blank_count=1;
			segment_digit=0;	//1桁目
			break;
		case 1:
			blank_count=2;
			segment_digit=0;	//1桁目
			break;
		case 2:
			blank_count=3;
			segment_digit=0;	//1桁目
			break;
		case 3:
			blank_count=4;
			segment_digit=2;	//表示ブランク
			break;
		case 4:
			blank_count=5;
			segment_digit=1;	//2桁目
			break;
		case 5:
			blank_count=6;
			segment_digit=1;	//2桁目
			break;
		case 6:
			blank_count=7;
			segment_digit=1;	//2桁目
			break;
		case 7:
			blank_count=0;
			segment_digit=2;	//表示ブランク
			break;
	}
	
	
	GPIOB->ODR=digit[segment_digit];	//7segの表示桁を切り替える
	GPIOA->ODR=seven_segment_led[segment_digit]<<4;	//7segに数字を表示


}







