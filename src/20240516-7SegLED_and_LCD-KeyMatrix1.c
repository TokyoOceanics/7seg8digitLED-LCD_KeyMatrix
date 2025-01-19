/*
 * 20240516-7SegLED_LCD-KeyMatrix1.c
 *
 *  Created on: 2024/05/16
 *      Author: x-lib
 *
 *
 *　LCDモジュールと7SegLED×8桁表示器とキーマトリクス動作確認プログラム
 *
 *　河原先生のボード、秋月電子の４ｘ３キーマトリクスを使用

 * 2024/05/06
 * keypad_getkeyにおいて、colの値が反映されないので、正しくキーコードが得られない。
 *
 * 配線の接触不良により、LCD表示が乱れる。これはループ先頭でLCD初期化をすることで対応。
 *
 *2024/05/08
 *スイッチマトリクスの検討
 *状況変わらず。加えて、LCD表示が著しく乱れるようになる。
 *
 *2024/05/10
 *校で不具合解析、スレッショルド不整合問題の一時しのぎ対策でのアーシング強化（横溝先生指摘による）
 *ピンの指し間違い、
 *
 *keypad_getkey関数におけるxyz信号線のビットセット・リセットの勘違い(リセットだけやっていた）
 *
 *2024/05/12 祥月命日
 *キーの打鍵が、正しくLCDに反映されるようになる。しかし、いまいちの動作あり。
 *桁選択情報COL値が意図した値になっていないが、押下キーに対する返り値はつじつまが合っている。
 *相変わらず、だいぶ安定はしたものの、No29号マイコンにおいては、LCDの表示が不安定。
 *
 *中継基板試作1号機がおかしいのではないかとなってきた。
 *
 *チャタリング防止策
 *
 *
 *2024/05/13
 *試作1号機において、LCD周りの配線とGNDとのピンを検査し、接触不良を発見、
 *これに対応したところ、ボードは動作するようになった。
 *
 *2024/05/16
 *ようやく、チャタリング防止を考慮するキー入力が動作
 *
*/

/*
 * 4x3キーパッド（キーマトリクス）の配線
 *
 * A	PA10	CN10-33
 * B	PA9		CN10-21
 * C	PA8		CN10-23
 * D	PA7		CN10-15
 *
 * Z	PA6		CN10-13
 * Y	PA5		CN10-11
 * X	PA4		CN7-32
 */



/*
 *　LCDモジュールの配線
 *
 * 	D7	PB15	CN10-26	(7SegmentLEDモジュールのSegment DPと兼用）
 * 	D6	PB14	CN10-28	(7SegmentLEDモジュールのSegment Gと兼用）
 * 	D5	PB13	CN10-30	(7SegmentLEDモジュールのSegment Fと兼用）
 * 	D4	PB12	CN10-16	(7SegmentLEDモジュールのSegment Eと兼用）
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



uint8_t keypad_kbhit();
uint8_t keypad_getkey();
void read_key(void);




void Debug_Blink_OnBoard_LED(int , int);


#define LCD_RS	7
#define LCD_EN	6

#define DELAY	1000




#define UserButtonSW	(GPIOC->IDR)&(0x01<<13)

#define PUSHED	0xffff


//グローバル変数
int TimeInt=0;	//delay処理（Waitする間隔を指定するため）のグローバル変数（SysTickで減算）

int key_pushed_flag=0; 			//キーパッド押下検出フラグ

uint8_t determined_key_code;	//現時点でのスイッチの確定値




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






/*
 *
 *　LCDに文字列を表示
 *　LCDに文字列を表示7セグLEDに一桁ずつ0-3を表示して、これを桁０から桁７まで繰り返す
 *
 */


//int main()
//{
//
//	my_init_system();//ハードウェアの初期化
//
//	loop:
//
//	lcd_init();
//
//	lcd_sendData(0x01);				//LCD clear
//	lcd_gotoxy(1,1);				//move cursor to 1,1
//	lcd_print("The world is but");	//show "The world is but"
//	lcd_gotoxy(1,2);				//move corsor to 1,2
//	lcd_print("one country");		//show "one country"
//
//
//	while(1){
//		for(int digit=0; digit<=7; digit++){
//			SelectDigit(digit);
//			for(int data=0; data<4; data++){
//				Display_7Seg(data);
//				Wait_Time(100);
//
//			}
//		}
//	}
//
//	goto loop;
//}





//int main()
//{
//
//	my_init_system();//ハードウェアの初期化
//
//	lcd_init();
//	lcd_gotoxy(1,1);	/* move cursor to 1,1 */
//	lcd_print("The pressed key is: "); /* show the message */
//
//	while(1)
//	{
//
////		uint8_t data=keypad_getkey();
//
//		if(keypad_kbhit() == 0){
//			lcd_gotoxy(1,2);	/* move cursor to 1,2 */
//			lcd_print("-");
//			lcd_gotoxy(4,2);	/* move cursor to 4,2 */
//			lcd_print("-");
//
//		}else{
//			lcd_gotoxy(1,2);	/* move cursor to 1,2 */
//			lcd_sendData(keypad_getkey()); /* show the pressed key */
//
//#ifdef	DEBUG
//			lcd_gotoxy(4,2);	/* move cursor to 4,2 */
//
//			lcd_sendData(COL+'0');
//		}
//#endif
//	}
//}
//


/*
 *
 * キー入力を受け付けて、LCDに表示する。
 * チャタリング防止機能付き。
 * 2024/05/16 23:00ようやく動いた。
 *
 *
 */


int main(void){


	int i=1;

	my_init_system();//ハードウェアの初期化

	lcd_init();
	lcd_gotoxy(1,1);	/* move cursor to 1,1 */
	lcd_print("The pressed key is: "); /* show the message */

	while(1){
		while(key_pushed_flag!=PUSHED);
		key_pushed_flag=0;

		lcd_gotoxy(i++,2);
		lcd_sendData(determined_key_code);

		if(i>16) i=1;

	}


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
	/*使い慣れた、ビットセット、リセットのやり方を使う。*/
	GPIOB->ODR&=~0xf000;			//上位4ビットの書き込み
	GPIOB->ODR|=(value<<8)&0xf000;
	delay_us(DELAY);

	lcd_strobe();

	GPIOB->ODR&=~0xf000;			//下位4ビットの書き込み
	GPIOB->ODR|=(value<<12)&0xf000;

	lcd_strobe();
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
	/*SysTick タイマ割込みの間隔を 10ms に設定 */
	// 割り込み間隔はSysTick_Config()の引数をRCC_Clocks.HCLK_Frequency / 1000　として、1msec
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);	//

	//GPIO　A,B,Cを有効化
	RCC->APB2ENR|=RCC_APB2ENR_IOPAEN|RCC_APB2ENR_IOPBEN|RCC_APB2ENR_IOPCEN;

	//TIM3 有効化して、PB0をTIM3とする
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;	//TIM3の有効化

	//			 76543210
	GPIOA->CRL=0x41114444;
	//			 FEDCBA98
	GPIOA->CRH=0x44444444;


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




/* The function checks if a key is pressed or not */
/* Returns: 0: Not pressed, 1: Pressed */
uint8_t keypad_kbhit()
{
				   //FEDCBA9876543210
	GPIOA->ODR &= ~0b0000000001110000;		//A4,A5,A6をLowにする
	delay_us(1);
	if(((GPIOA->IDR>>7)&0xF) == 0xF) /* 何もキーが押されていない場合は0を返す*/
		return 0;
	else
		return 1;					/*何か押されていれば、1を返す*/
}



/* The function returns the pressed key. */
/* It returns 0 if no key is pressed. */

uint8_t keypad_getkey()
{
						// ColX ColY ColZ
	uint8_t keypadLookup[]={'1','2','3',	//RowD
							'4','5','6',	//RowC
							'7','8','9',	//RowB
							'*','0','#'};	//RowA
//	uint8_t character_code;

	const uint32_t colSelect[]={
		0x00100060, /* ColX ZYX=001 */
		0x00200050, /* ColY ZYX=010 */
		0x00400030};/* ColZ ZYX=100 */

	if(keypad_kbhit() == 0)	/* if no key is pressed */
		return 0;

	for(int col = 0; col <= 2; col++) /* col 0 to 2 */
	{
		GPIOA->BSRR=  colSelect[col]; /*　ZYXセレクトしている列だけをLにする */
		delay_us(10);	/* wait for the columns to beupdated */
		uint8_t row = (GPIOA->IDR>>7)&0xF;

		switch(row)
		{
			case 0x0E:  /* rowD is low */
				return keypadLookup[0+col];

			case 0x0D: /* rowC is low */
				return keypadLookup[3+col];

			case 0x0B: /* rowB is low */
				return keypadLookup[6+col];

			case 0x07: /* rowA is low */
				return keypadLookup[9+col];

		}
	}

	return 0; //返り値が0の場合は、押しボタンの押下無し。この行はコンパイル時の警告抑制用に記述。
}


/*
 * ３ｘ４キーパッドのキー押下状況の取り込み関数
 *
 * チャタリング防止機能付き。 SysTick_Handlerから呼び出して使用する。
 *
 *　上位関数にて、キーバッド押下フラグ、現時点で押下されたキーコードの確定値を調べ、キー入力の判断をする。
 *
 * キーパッド押下フラグkey_pushed_flag
 * 0の時は押下無し、PUSHEDなら押下あり。
 *
 * 現時点で押下されたキーコードの確定値determined_key_code
 * 押されているキーのキー表面に印刷された文字コードもしくは、押下無しなら0。
 *
 */
void read_key(void){
	static uint8_t last_determined_key_code;	//前回までのスイッチの値の確定値

	static uint8_t current_key_code =0;	//今回読み込んだスイッチの値
	static uint8_t previous1_key_code=0;//前回読み込んだスイッチの値
	static uint8_t previous2_key_code=0;//前々回読み込んだスイッチの値



	previous2_key_code=previous1_key_code;	//直近3つのスイッチの値を、玉突きで更新
	previous1_key_code=current_key_code;
	current_key_code=keypad_getkey();

	//直近3個のスイッチの値がすべて等しければ、それを確定値として保持する。
	if( (current_key_code==previous1_key_code) && (previous1_key_code==previous2_key_code)){
		determined_key_code=current_key_code;
	}

	//以下のロジック、スイッチの押下を検出したときの、前回の確定値の扱い、
	//立上りフラグの扱いは苦労した。2024/05/16 23:00
	//いまのところ、同一キー入力が受け入れられない状況。

	//直前までの確定値と直近の確定値が等しくなければ、
	//現時点での確定値が０以外ならば、意味のあるキーの押下があったものとして、
	//押下フラグと、前回の確定値とを更新する。
	if((determined_key_code!=last_determined_key_code)||(last_determined_key_code==0)){
//	if((last_determined_key_code==0)&&(determined_key_code!=0)){
	if(determined_key_code!=0){
			key_pushed_flag=PUSHED;
			last_determined_key_code=determined_key_code;
		}
	}

	return;

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

	//３ｘ４キーパッドのキー押下状況の取得
	read_key();


	//
	//待ち時間関数　Wait_Time()用の変数の処理
	//
	if(TimeInt>0) TimeInt--;
}


/*
 * デバッグ用 F103ボードのオンボードLEDを点灯させる。
 *
 */
void Debug_Blink_OnBoard_LED(int count, int delay)
{
	for(int i=0; i<count; i++){
		GPIOA->ODR^=(0x1<<5);
		Wait_Time(delay);
	}
}
