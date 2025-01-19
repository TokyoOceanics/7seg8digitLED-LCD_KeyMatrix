/*
 * 20240413-7SegLED_and_LCD-01.c
 *
 *  Created on: 2024/04/13
 *      Author: x-lib
 *
 *
 *　LCDモジュールと7SegLED×8桁表示器の動作確認プログラム
 *
 *　河原先生のボードを流用する
 *
 *	LCD表示制御がうまくいかず、いろいろ悩む。
 *	lcd_putValue関数で、BRR、BSRRを使うと不具合が起こる。今一つこの2つのレジスタを理解できていない。
 *	＆、｜でビットをリセット、セットするやり方で、動作するようになった。
 *	LCD_strobe関数で、チョンボをしていて、気が付くのに半日を要した。
 *
 *	配線数が多いため、配線ミスも悩ましい状況を呼んだ
 */


/*
 *　LCDモジュールの配線
 *
 * 	D7	PB15	CN10-26
 * 	D6	PB14	CN10-28
 * 	D5	PB13	CN10-30
 * 	D4	PB12	CN10-16
 *
 * 	RS	PB7		CN7-21
 * 	E	PB6		CN10-17
 *
 *
 */


/*
 * 7 SegmentLEDモジュール、ブザの配線
 *
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




#include <stm32f10x.h>

//関数のプロトタイプ宣言


void lcd_init(void);
void lcd_sendCommand(unsigned char cmd);
void lcd_sendData(unsigned char data);
void lcd_print(char * str);
void lcd_gotoxy(unsigned char x, unsigned char y);

void delay_us(uint16_t t);



void my_init_system(void);
void Wait_Time(int time);	//SysTick_Handlerを使う待ち時間処理

void SelectDigit(int digit);
void Display_7Seg(int number);

void Debug_Blink_OnBoard_LED(int , int);


#define LCD_RS	7
#define LCD_EN	6

#define DELAY	1000




#define UserButtonSW	(GPIOC->IDR)&(0x01<<13)



//グローバル変数
int TimeInt=0;	//delay処理（Waitする間隔を指定するため）のグローバル変数（SysTickで減算）








//int main()
//{
//
//	my_init_system();//ハードウェアの初期化
//
//	lcd_init();
//
//	lcd_gotoxy(1,1);				//move cursor to 1,1
//	lcd_print("The world is but");	//show "The world is but"
//	lcd_gotoxy(1,2);				//move cursor to 1,2
//	lcd_print("one country");		//show "one country"
//
//	Debug_Blink_OnBoard_LED(3, 500);
//	while(1);
//}














//
//メイン関数
//

/*
 *  *を永遠に表示する。
 *  カーソル位置を1文字づつ右に動かしながら。
 *
 */


//int main(void){
//
//
//	my_init_system();
//	lcd_init();
//
//	while(1){
//	for(int line=1; line<=2; line++)
//		for(int col=1; col<=16; col++){
//			lcd_gotoxy(col,line);
//			lcd_sendCommand(0x0e);	//display on, cursor on
//			lcd_sendData('*');
//			Debug_Blink_OnBoard_LED(2,100);
//			Wait_Time(300);
//			lcd_sendCommand(0x01);
//		}
//
//
//	}
//}



///*
// * ABC、と順番にアルファベットを液晶に表示する。
// * Q以降は2行目に表示する。
// *
// */
//
//int main(void){
//
//	my_init_system();
//	lcd_init();
//
//
//
//	lcd_gotoxy(1,1);
//
//	for(uint8_t val='A'; val<='Z'; val++){
//		if((val-'A')==16)	//Q以降は2行目に表示する。
//			lcd_gotoxy(1,2);
//		lcd_sendData(val);
//		Debug_Blink_OnBoard_LED(2,100);
//
//	}
//
//	while(1)
//		Debug_Blink_OnBoard_LED(1,50);
//
//
//
//}









int main()
{

	my_init_system();//ハードウェアの初期化


	lcd_init();

loop:
	lcd_sendData(0x01);				//LCD clear
	lcd_gotoxy(1,1);				//move cursor to 1,1
	lcd_print("The world is but");	//show "The world is but"
	lcd_gotoxy(1,2);				//move corsor to 1,2
	lcd_print("one country");		//show "one country"


	while(1){
		for(int digit=0; digit<=7; digit++){
			SelectDigit(digit);
			for(int data=0; data<4; data++){
				Display_7Seg(data);
				Debug_Blink_OnBoard_LED(2,200);
			}
		}
	}

	goto loop;
}






/*
 * LCDのデータ書き込みパルス（ストローブ信号）発生　lcd_putValue関数から呼び出される
 *
 * LCDモジュールのD7～D4上にデータ確定した後、ストローブ信号ENの立下りパルスを出せば、
 * そのデータが書き込まれる。
 */


void lcd_strobe(void)
{
	//ストローブ信号（Eの立下り）で、LCDのレジスタにデータを転送
	GPIOB->ODR |= (1<<LCD_EN);	// EN=1 for H-to-L pulse
	delay_us(DELAY);
	GPIOB->ODR &= ~(1<<LCD_EN);	// EN=0 for H-toL pulse
	delay_us(DELAY);


}

/*
 * LCDモジュールへデータを書き込む関数
 *
 * 引数で指定されるvalueの上位4ビットを書き込み、次に下位4ビットを書き込む。
 * この関数は、LCDモジュール表示制御関数の一番低いレイヤーとして機能する。
 */

void lcd_putValue(unsigned char value)
{
#if 0	/*このブロックは、いろいろ試したが、うまく動作しない*/
//	uint16_t debug_value;

	//valueの上位4ビットをPB15-PB12（LCDのD7-D4）に出力する
	//
	GPIOB->BRR=0xf0000000;	//PB15-PB12をゼロクリアする
	//                       3210
	GPIOB->BSRR=(value<<8)&0xf000;	//valueの上位4ビットを PB15-PB12へ出力
//	debug_value=(value<<8)&0xf000;
	delay_us(DELAY);

	lcd_strobe();

	//valueの下位4ビットデータをPB15-PB12（LCDのD7-D4）に出力する
	GPIOB->BRR=0xf0000000;	//PB15-PB12をゼロクリアする
	GPIOB->BSRR=(value<<12)&0xf000;	//valueの下位４ビットをPB15-PB12へ出力
//	debug_value=(value<<12)&0xf000;

	delay_us(DELAY);

	lcd_strobe();
#else
	/*使い慣れた、ビットセット、リセットのやり方を使う。*/
	GPIOB->ODR&=~0xf000;			//上位4ビットの書き込み
	GPIOB->ODR|=(value<<8)&0xf000;
	delay_us(DELAY);

	lcd_strobe();

	GPIOB->ODR&=~0xf000;			//下位4ビットの書き込み
	GPIOB->ODR|=(value<<12)&0xf000;

	lcd_strobe();

#endif


}


/*
 * LCDモジュールに対して、制御コマンドを送る関数
*/
void lcd_sendCommand(unsigned char cmd)
{
	GPIOB->ODR &= ~(1<<LCD_RS);	//RS =0 for command
	delay_us(DELAY);

	lcd_putValue(cmd);

}

/*
 * LCDモジュールに対して、データ(主に文字）を送る関数
*/

void lcd_sendData(unsigned char data)
{
	GPIOB->ODR |=(1<<LCD_RS);	//RS=1 for data
	delay_us(DELAY);

	lcd_putValue(data);


}

/*
 * LCDモジュールに文字列を表示する関数
 */
void lcd_print(char* str)
{
	unsigned char i=0;
	while(str[i]!=0){
		lcd_sendData(str[i]);
		i++;
	}
}


/*
 * LCDモジュールを初期化する関数
 */
void lcd_init()
{
	GPIOB->ODR &=~(1<<LCD_EN);	//LCD_EN=0
	delay_us(DELAY);

#if 0
		//山田先生の初期化シーケンス
		//0x30 -> 0x30 -> 0x30 -> 0x20
		lcd_sendCommand(0x33);	//8bit mode 7654
								//          0011
		lcd_sendCommand(0x32);	//functionSet 4bit

		lcd_sendCommand(0x28);	//functionSet 4bit, 2line, 5x7font　冗長ながら、4ビットモードを今一度実施


		lcd_sendCommand(0x08);	//dispTurnOff, cursorOff, blinkOff
		lcd_sendCommand(0x01);	//dispClear
		lcd_sendCommand(0x06);	//entryMode cursorMoveRight
		lcd_sendCommand(0x0f);	//dispTurnOn

#else


	//書籍オリジナルのシーケンス
	//0x30 -> 0x30 -> 0x30 -> 0x20
	lcd_sendCommand(0x33);	//8bit mode 7654
							//          0011
	lcd_sendCommand(0x32);	//functionSet 4bit

	lcd_sendCommand(0x28);	//functionSet 4bit, 2line, 5x7font　冗長ながら、4ビットモードを今一度実施

	lcd_sendCommand(0x0e);	//display on, cursor on
	lcd_sendCommand(0x01);	//clear LCD
	delay_us(DELAY);
	lcd_sendCommand(0x06);	//shift cursor right


#endif

}

/*
 * 待ち時間関数（アイドルループによる）
 */
void delay_us(uint16_t t)
{
	volatile unsigned long l=0;
	for(uint16_t i=0; i<t; i++)
		for(l=0; l<6; l++);
}


/*
 * カーソルを指定位置に移動する関数
 *　xは桁位置、yは行
 */
void lcd_gotoxy(unsigned char x, unsigned char y)
{
	const unsigned char firstCharAdr[]={0x80,0xc0, 0x94,0xd4};
	lcd_sendCommand(firstCharAdr[y-1]+x-1);
	delay_us(100);
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

	//GPIO　B,Cを有効化
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN|RCC_APB2ENR_IOPCEN;	// GPIOB,Cのクロックを有効にする

	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;	//GPIOA デバッグ用に有効化

	//TIM3 有効化して、PB0をTIM3とする
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;	//TIM3の有効化

	//ポートA　デバッグ用
	//			 76543210
	GPIOA->CRL=0x44144444;



	//ポートBの入出力設定　GPIOB->CRL設定は、ポートBの入出力設定　pb0をTIM3とする
	//PB0　CNF=10、MODE=11は　オルタネート出力・プッシュプル
	//			 76543210
	GPIOB->CRL=0x1144444B;
	//			 FEDCBA98
	GPIOB->CRH=0x11111111;

	//ポートC
	//			 76543210
	GPIOC->CRL=0x11111111;
	//			 FEDCBA98
	//GPIOC->CRH=0x44444444;



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


/*
 * デバッグ用
 *
 */
void Debug_Blink_OnBoard_LED(int count, int delay)
{
	for(int i=0; i<count; i++){
		GPIOA->ODR^=(0x1<<5);
		Wait_Time(delay);
	}
}
