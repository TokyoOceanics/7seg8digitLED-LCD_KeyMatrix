/*
 * 	Sw1,Sw2のテスト
 * 	Sw1で7segの1桁目を全点灯
 * 	Sw2で7segの2桁目を全点灯
 * 	SW1,Sw2同時押しで、7segを全点灯
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


#define Delay_Time	10	//Wait時間　1ms単位

#define Sw1 0x01	//Sw1(PC0)用のマスク値
#define Sw2 0x02	//Sw1(PC1)用のマスク値



int set_segment(int value);
int num_to_2digit(int num,int *digit_one, int *digit_ten);

//
//グローバル変数
//
int TimeInt=0;	//delay処理（Waitする間隔を指定するため）のグローバル変数（SysTickで減算）

int up=0;		//スイッチの立ち上がり検出フラグ、0で初期化
int sw_now=0;	//現在のスイッチ押下状態、0で初期化


int main(void)
{

	my_init_system();

	int count=0;	//カウンタ変数


	while(1){
		set_segment(count);	//カウンタの値を７セグメント表示器に表示

		switch(GPIOC->IDR&(Sw1|Sw2)){
			case Sw1:
				GPIOA->ODR=0;
				GPIOB->ODR=digit[0];
				break;
			case Sw2:
				GPIOA->ODR=0;
				GPIOB->ODR=digit[1];
				break;
			case Sw1|Sw2:
				GPIOA->ODR=0;
				GPIOB->ODR=~digit[0]|~digit[1];
				break;


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
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);	// SysTick タイマ割込みの間隔を 1ms に設定


	//GPIO　A,B,Cを有効化
	RCC->APB2ENR = RCC_APB2ENR_IOPAEN |	// GPIOA のクロックを有効にする
				   RCC_APB2ENR_IOPBEN |	// GPIOB のクロックを有効にする
				   RCC_APB2ENR_IOPCEN;	// GPIOC のクロックを有効にする

	//ポートA,B,Cの入出力設定

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

	
	int segment_digit;	//7セグ表示桁位置番号　0で1桁目、1で2桁目、3で消灯
	static int blank_count=0;//7セグ表示桁の消灯制御用の状態変数、詳細は下記switch-case参照

	//SysTick_Handlerによるスイッチのチャタリング除去用のフラグ類
	static int swr2=0;	//２サンプル単位前のスイッチ状態を格納する変数
	static int swr1=0;	//１サンプル単位前のスイッチ状態を格納する変数
	static int swr0=0;	//現時点でサンプルしたスイッチ状態を格納する変数

	static int sw_last=0;	//前回確定させたスイッチ押下状態

	//
	//スイッチのチャタリング除去の処理
	//

	swr2=swr1;	//2サンプル前のスイッチ状態として、1サンプル前のスイッチ状態を退避格納
	swr1=swr0;	//1サンプル前のスイッチ状態として、現時点のスイッチ状態を退避格納

	swr0=GPIOC->IDR&(Sw1|Sw2);	//現時点のスイッチ状態を取得し、必要部分をマスク抽出

	if((swr2==swr1)&&(swr1==swr0)){//2サンプル前、1サンプル前、現時点のスイッチ状態が等しいなら、それを現時点のスイッチ状態として確定
		sw_now=swr0;
	}

	//スイッチ押下の検出（立上り検出）
	if((sw_now!=sw_last)){	//現時点のスイッチ状態確定値と、前回のスイッチ状態確定値が一致しない場合
		up=sw_now&~sw_last;	//立上り検出フラグを設定
		sw_last=sw_now;		//現在のスイッチ状態確定値を更新
	}


	//
	//待ち時間関数　Wait_Time()用の変数の処理
	//

	//割り込み間隔１ｍｓ、1回のダウンカウントに１ｍｓ
	if(TimeInt>0) TimeInt--;

	

	//
	//7セグLED表示処理
	//

	//各桁の表示切替と、表示切替の瞬間にいったん全桁表示OFFのブランクを入れる処理
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







