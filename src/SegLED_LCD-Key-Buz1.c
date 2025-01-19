/*
 * 7SegLED_LCD-Key-Buz1.c
 *
 *  Created on: 2024/10/08
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
 *しかし、同じキー入力を受け付けない不具合
 *
 *2024/05/18 01:38
 *ようやく動作した。
 *
 *2024/06/02 キー入力を数値とする処理を追加、数値が表示されるようになった。
 *
 *collatzの処理を入れて、動作させる。ＬＣＤ上に動作状態を表示させる。　06/02 17:40
 *
 *
 *2024/06/03
 *Buzzer処理追加に着手
 *2024/06/04
 *動作し始める。変数をbuzzer.hにて定義していて、2重定義でエラーが出ていたのを回避した。
 *キーパッド打鍵音がいまいちうまくいかない。
 *
 *2024/06/05
 *キーパッド打鍵音が出るようになる。なぜか、keypad_clack_sound関数内において、
 *SysTickを使う割り込みWait_Timeを使うと、暴走的状況となる。
　*やむを得ず、アイドルループでディレイをdelay_usを転用したところ、意図する動作となる。

 20240603-

 *2024/06/06
 *7Seg検討開始

 2024/06/29 再開
 キー入力を始めて、＃の次に打鍵する数字キー打鍵後に
Queue_to_number_array(&queue, num_on_7segLED);
に突入すると、Hard Faultする。

2024/07/01
キー入力過程の状況を7セグＬＥＤ上に表示させることが出来るようになる。
しかし、計算過程の表示を7セグＬＥＤ上に表示させると、ＬＣＤ表示が壊れることが発覚。
同時に、ＬＣＤとＬＥＤとは制御ができないのか。read modfy writeの技は使っているつもりだが。
chatGPTの生成するコードも利用してみたが、いまいちの様子。


2024/07/03
表示の不具合は、もしかしたら、Buz（PB0）にあるのかもしれない。

2024/07/11
この版でまずは固定し、教材として提供


2024/09/06
get_numberの状態遷移設計の見直し
バグ収まらず。

2024/09/08
get_numberの見直しを再開。されど、例外入力時のリカバリーに不具合あり。

2024/09/10
get_number関数内での、再入力処理（＊押下）をあきらめて動作を単純化

2024/10/03 T.Yamada
lcdへのデータ送信時(ODR書き込み+strobe)に以下を行う。
	・7Seg表示更新処理を止める。enable_segment_display<-DISABLE or ENABLEを
	　利用する。
	・LCDと7SegのODR兼用ビットに対する操作を復元して、LCDの操作がなかったことにする。兼用4
	　ビットはLCD側の処理では転送のための一過性の利用であるので、そのデータはstrobe後は
	　必要としない。これに対して、7Seg側は表示のために兼用4ビット上に存在する必要があるため、
	　LCDの処理が済んだら、速やかに7Segデータを復元して置く必要がある。

上記の改善で変化がなければ、以下を行う。
	・キーの入力待ちをなくして、メインループで7Seg表示更新を行うようにする。
	・キーの入力を1ms毎ではなく、10ms毎に変える。



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
 *							//y LCD 兼用
 *SegDP	PB15	CN10-26     // D7
 *SegG	PB14	CN10-28     // D6
 *SegF	PB13	CN10-30     // D5
 *SegE	PB12	CN10-16		// D4
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




#include <queue_tool.h>
#include <buzzer.h>
#include <stm32f10x.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//関数のプロトタイプ宣言


void lcd_init(void);
void lcd_sendCommand(unsigned char cmd);
void lcd_sendData(unsigned char data);
void lcd_print(char * str);
void lcd_gotoxy(unsigned char x, unsigned char y);
void lcd_all_clear(void);
void lcd_clear_line(int line);

void delay_us(uint16_t t);



void my_init_system(void);
void Wait_Time(int time);	//SysTick_Handlerを使う待ち時間処理

void SelectDigit(int digit);
void Display_7Seg(int number);



uint8_t keypad_kbhit();
uint8_t keypad_getkey();
void read_key(void);
void wait_any_key_input(void);

int get_number(int *number);
//int disp_queue_on_7_segment_LED(QUEUE_T*);
void Queue_to_number_array(QUEUE_T* queue, int num[]);

int Mister_Collatz(int start_number);


void Debug_Blink_OnBoard_LED(int , int);


#define LCD_RS	7
#define LCD_EN	6

#define DELAY	1000


/****************************************************************/
#define DISP_7SEG	//7SegmentLEDに計算途上を表示する場合は、コメントアウトする。
/****************************************************************/

#define UserButtonSW	(GPIOC->IDR)&(0x01<<13)

#define PUSHED	0xffff
#define ENABLE	0xff
#define DISABLE	0x0


//グローバル変数
int TimeInt=0;	//delay処理（Waitする間隔を指定するため）のグローバル変数（SysTickで減算）

int key_pushed_flag=0; 			//キーパッド押下検出フラグ

uint8_t determined_key_code;	//現時点でのスイッチの確定値

int num_on_7segLED[8];	//7 セグメントＬＥＤに表示する数値

int enable_segment_display=DISABLE;	//7セグメントＬＥＤ表示許可


//楽曲の音符データ
const Note melody[][20]={
		//安冨先生
		{{D5,LEN1},{B4,LEN1},{A4,LEN1},{G4,LEN1},{F4,LEN1},{E4,LEN1},
				{F4,LEN1},{C5,LEN1},{C5,LEN1},
				{B4,LEN1},{A4,LEN1},{G4,LEN1},{F4,LEN1},{F4,LEN1},{E4,LEN1},{D4,LEN1},{C4,LEN1},{C4,LEN1},{0,0}},
		//ファミリーマート
		{{B4,LEN1},{G4,LEN1},{D4,LEN1},{G4,LEN1},{A4,LEN1},{D5,LEN1*2},
				{D4,LEN1},{A4,LEN1},{B4,LEN1},{A4,LEN1},{D4,LEN1},{G4,LEN1*2},{0,0}},
		//きらきら星
		{{C4,LEN1},{C4,LEN1},{G4,LEN1},{G4,LEN1},{A4,LEN1},{A4,LEN1},{G4,LEN1*2},
				{F4,LEN1},{F4,LEN1},{E4,LEN1},{E4,LEN1},{D4,LEN1},{D4,LEN1},{C4,LEN1*2},{0,0}},
		//
		{{C4,LEN1},{D4,LEN1},{E4,LEN1},{F4,LEN1},{G4,LEN1},{A4,LEN1},{B4,LEN1},{C5,LEN1},{D5,LEN1*2},{0,0}},
		//
		{{D5,10},{C4,10},{0,0}}

	};

#if 0
/*
 * https://nicerland.com/stm32f103/
 * の9章のプログラムの転用
 */
int main()
{

	my_init_system();//ハードウェアの初期化

	lcd_init();

	lcd_gotoxy(1,1);				//move cursor to 1,1
	lcd_print("The world is but");	//show "The world is but"
	lcd_gotoxy(1,2);				//move cursor to 1,2
	lcd_print("one country");		//show "one country"

	Debug_Blink_OnBoard_LED(3, 500);
	while(1);
}
#endif













#if 0
/*
 *  ＬＣＤ表示器に*を永遠に表示する。
 *  カーソル位置を1文字づつ右に動かしながら。
 *
 */

int main(void){


	my_init_system();
	lcd_init();

	while(1){
	for(int line=1; line<=2; line++)
		for(int col=1; col<=16; col++){
			lcd_gotoxy(col,line);
			lcd_sendCommand(0x0e);	//display on, cursor on
			lcd_sendData('*');
			Debug_Blink_OnBoard_LED(2,100);
			Wait_Time(300);
			lcd_sendCommand(0x01);
		}
	}
}

#endif



#if 0
/*
 *
 *　LCDに文字列を表示
 *　LCDに文字列を表示7セグLEDに一桁ずつ0-3を表示して、これを桁０から桁７まで繰り返す
 *
 */


int main()
{

	char str[17];

	my_init_system();//ハードウェアの初期化

	loop:

	lcd_init();

	lcd_sendData(0x01);				//LCD clear
	lcd_gotoxy(1,1);				//move cursor to 1,1
	lcd_print("The world is but");	//show "The world is but"
	lcd_gotoxy(1,2);				//move corsor to 1,2
	lcd_print("one country");		//show "one country"


	while(1){
		for(int digit=0; digit<=7; digit++){
			SelectDigit(digit);
			for(int data=-1; data<4; data++){



				lcd_all_clear();
				sprintf(str,"[%d]",data);
				lcd_gotoxy(1,2);
				lcd_print(str);

				Display_7Seg(data);


				Wait_Time(500);

			}
		}
	}

	goto loop;
}

#endif



#if 0
/*
 *
 *　LCDに文字列を表示
 *　LCDに文字列を表示7セグLEDに一桁ずつ0-3を表示して、これを桁０から桁７まで繰り返す
 *
 */


int main()
{

	my_init_system();//ハードウェアの初期化

	loop:

	lcd_init();

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
				Wait_Time(100);

			}
		}
	}

	goto loop;
}

#endif







#if 0
/*
 * キーボードからの入力をＬＣＤに表示
　*　https://nicerland.com/stm32f103/の9章のプログラムを秋月のキーパッドに移植
 */

int main()
{

	my_init_system();//ハードウェアの初期化

	lcd_init();
	lcd_gotoxy(1,1);	/* move cursor to 1,1 */
	lcd_print("The pressed key is: "); /* show the message */

	while(1)
	{

//		uint8_t data=keypad_getkey();

		if(keypad_kbhit() == 0){
			lcd_gotoxy(1,2);	/* move cursor to 1,2 */
			lcd_print("-");
			lcd_gotoxy(4,2);	/* move cursor to 4,2 */
			lcd_print("-");

		}else{
			lcd_gotoxy(1,2);	/* move cursor to 1,2 */
			lcd_sendData(keypad_getkey()); /* show the pressed key */

#ifdef	DEBUG
			lcd_gotoxy(4,2);	/* move cursor to 4,2 */

			lcd_sendData(COL+'0');
#endif

		}
	}
}

#endif




#if 0
/*
 *
 * キー入力を受け付けて、LCDに表示する。
 * チャタリング防止機能付き。
 * 2024/05/16 23:00ようやく動いた。
 *
 *
 */


int main(void){


	int i=1;	//打鍵した数字の表示桁位置をいちばん左側、行頭にしておく。

	my_init_system();//ハードウェアの初期化

	lcd_init();
	lcd_gotoxy(1,1);	/* カーソルを1行目の1列目、すなわち行頭に移動する */
	lcd_print("The pressed key is: "); /* メッセージを表示(いわゆるプロンプト） */

	while(1){
		while(key_pushed_flag!=PUSHED);	//キー入力があるまで待つ。
		key_pushed_flag=0;				//キー入力があったので、押下フラグをクリアする。

		lcd_gotoxy(i++,2);				//2行目のi列に印字し、次の文字を表示する位置を一文字分右に移動する
		lcd_sendData(determined_key_code);//文字を表示する。

		if(i>16) i=1;	//行末まで表示したら、行頭に戻す

	}
}

#endif



#if 0
/*
 * Buzzer テスト
 */
int main(void){

	my_init_system();

	melody_address=melody[0];	//楽曲データの先頭アドレス
	play_music(melody_address);	//演奏

  	while(1);

}
#endif




#if 0

int main(void){
	while(1)
		for(int i=0; i<100; i++);
	while(1);
}

#endif


#if 1
/*
 *
 * キー入力を受け付けて、Collatz数列をLCDに表示する。
 *
 *
 */



int main(void){

	int number;						//キーパッドから取り込んだ数値
	int digit;						//取り込んだ数値の桁数
	char number_string[MAX_NUM];	//LCDに表示するための数値の数字列
	int collatz_count;
	char message[16+1];

	my_init_system();//ハードウェアの初期化

	lcd_init();

	while(1){


		lcd_all_clear();			/* プロンプト表示 */
		//lcd_init();
		lcd_gotoxy(1,1);
		lcd_print("Collatz Problem");
		lcd_gotoxy(1,2);
		lcd_print("++ﾐｶｲｹﾂ ﾓﾝﾀﾞｲ++");

		melody_address=melody[1];	//ファミリーマート入店音を演奏
		play_music(melody_address);	//

		do{

			digit=get_number(&number);	//キーパッドからの入力待ち。数字キー打鍵後、#で入力終了。

			sprintf(number_string,"%d", number);//取り込んだ数値を数字列に格納。

			lcd_clear_line(1);		//Collatz数列の計算開始のプロンプト
			lcd_gotoxy(1,1);
			lcd_print("Number is ");
			lcd_print(number_string);

		}while(digit==0);


		lcd_clear_line(2);
		lcd_gotoxy(1,2);
		lcd_print("Any key to Start");
		wait_any_key_input();


		collatz_count=Mister_Collatz(number);	//Collatz数列の計算


		sprintf(message,"[%d] mks %d cnts",number,collatz_count);//Collatz数列の計算結果表示
		lcd_clear_line(1);
		lcd_gotoxy(1,1);
		lcd_print(message);

		lcd_clear_line(2);
		lcd_gotoxy(1,2);
		lcd_print("Hit any key.");

		melody_address=melody[0];	//音を演奏
		play_music(melody_address);	//

		wait_any_key_input();


	}



}




//void keypad_clack_sound(void){
//
//	//キーボード打鍵音
//
////	Note mldy[]={{C4,10},{D4,10},{0,0}};
////	play_music(mldy);
//
////	int i=0;
////	for(i=0; i<10; i++);
//
//	play_tone(C4);
//	delay_us(1000);
//	play_tone(C5);
//	delay_us(1000);
//
//}



#endif












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
	//y ＬＥＤの表示を止める
	enable_segment_display = DISABLE;
	//y 7Seg用上位8ビットの現状退避 2024/10/06
	int16_t save = GPIOB->ODR | 0xff00;

	/*使い慣れた、ビットセット、リセットのやり方を使う。*/
	//y GPIOB->ODR&=~0xf000;	//上位4ビットの書き込み
	GPIOB->BRR = 0xf000;		//上位4ビットのゼロクリア
	//y GPIOB->ODR|=(value<<8)&0xf000;
	GPIOB->BSRR = (value << 8) & 0xf000;	//上位4ビットへのセット
	delay_us(DELAY);

	lcd_strobe();

	//y GPIOB->ODR&=~0xf000;	//下位4ビットの書き込み
	GPIOB->BRR = 0xf000;		//上位4ビットのゼロクリア
	//y GPIOB->ODR|=(value<<12)&0xf000;
	GPIOB->BSRR = (value << 12) & 0xf000;	//上位4ビットへのセット
	delay_us(DELAY); //y 追加

	lcd_strobe();

	//y 7Seg用上位8ビットの現状復帰 2024/10/06
	GPIOB->BRR = 0xff00;		//上位8ビットのゼロクリア
	GPIOB->BSRR = save;			//上位8ビットの復元
	//y ＬＥＤの表示を再開
	enable_segment_display = ENABLE;
}


/*
 * LCDモジュールに対して、制御コマンドを送る関数
*/
void lcd_sendCommand(unsigned char cmd)
{
	//y GPIOB->ODR &= ~(1<<LCD_RS);	//RS =0 for command
	GPIOB->BRR = (1<<LCD_RS);	//RS =0 for command
	delay_us(DELAY);

	lcd_putValue(cmd);

}

/*
 * LCDモジュールに対して、データ(主に文字）を送る関数
*/

void lcd_sendData(unsigned char data)
{
	//y GPIOB->ODR |=(1<<LCD_RS);	//RS=1 for data
	GPIOB->BSRR = (1<<LCD_RS);	//RS=1 for data
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

/*
 * LCD全行消去
 */
void lcd_all_clear(void)
{
	lcd_sendCommand(0x01);
}


/*
 * LCDの指定行の表示を消去　2024/06/02
 */
void lcd_clear_line(int line){
	lcd_gotoxy(1,line);
	lcd_print("                ");
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
	//ここを100にして、WaitTime(1000)で10秒待ちになる）
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);	//

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



    // PWMモードを選択し、PWM信号をピンに出力する
    TIM3->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2;
    TIM3->CCER |= TIM_CCER_CC3E;

    // プリスケーラを設定する
    TIM3->PSC = 720-1;

    // タイマを有効にするコントロールレジスタ。　タイマのON・OFFなど設定
    TIM3->CR1 |= TIM_CR1_CEN;

}




void SelectDigit(int digit)
{

	if( (0<=digit) && (digit<=7) )
		GPIOC->ODR=(0x01<<digit);
	else
		GPIOC->ODR=0x00;

}




/*
 * 安藤先生技証コード転用
 *
 *　ＬＣＤのEN(PB6),RS(PB7)に影響を与えないやり方の模索
 */
#if 0
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

		GPIOB->ODR &= ~(Seg7led_Table[number] /*& 0x7f00*/);	// 7 セグメント LED に表示する
		GPIOB->ODR |= (Seg7led_Table[number] /*& 0x7f00*/);
	} else {											// 0 ～ 9 の範囲外の値が指定された場合は
		GPIOB->ODR |= 0x00FF;
	}
}
#endif






/*
 * 安藤先生技証コード転用
 *
 *　ただし、ＬＣＤのEN(PB6),RS(PB7)を上書きしてしまうためか、ＬＣＤ表示が乱れる。
 */
#if 0
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
		GPIOB->ODR = Seg7led_Table[number] & 0x7f00;	// 7 セグメント LED に表示する
	} else {											// 0 ～ 9 の範囲外の値が指定された場合は
		GPIOB->ODR = 0x0000;							// ７セグメント LED を全消灯する
	}
}
#endif


#if 0
/*
 * 2024/07/01
 * ChatGPTの生成による版
 * 直下にあるコードをChatGPTに送り、下位8ビットに影響を与えないようにしたい旨をお願いしたら、
 * 生成されたコード。
 *
 * LEDに途中経過は表示されるが、最後に数字化け。加えて、LCD表示を破壊。
 */

void Display_7Seg(int number) {
    /*
    **      < 7 セグメント LED のポート割り当て >
    **
    **             a        a - PB8
    **           ----        b - PB9
    **        f / g / b        c - PB10
    **          ----        d - PB11
    **       e /   / c        e - PB12
    **         ----            f - PB13
    **          d            g - PB14
    */

    /* 7 セグメント LED 表示パターン用のデータ配列（正論理として定義） */
    const static int Seg7led_Table[] = { 0x3f00,    // 0
                                         0x0600,    // 1
                                         0x5b00,    // 2
                                         0x4f00,    // 3
                                         0x6600,    // 4
                                         0x6d00,    // 5
                                         0x7d00,    // 6
                                         0x2700,    // 7
                                         0x7f00,    // 8
                                         0x6f00     // 9
    };

    int temp_gpio;

    if ((number >= 0) && (number <= 9)) {                // 数値が 0 ～ 9 の範囲なら指定された数字のパターンを
        temp_gpio = Seg7led_Table[number] & 0x7f00;

        // 下位8ビットを保護しつつ上位8ビットをリセット
        //y GPIOB->ODR &= 0x00FF;
        // 下位8ビットを保護しつつ上位8ビットを設定
        //y GPIOB->ODR |= temp_gpio;
    	GPIOB->BSRR = temp_gpio;
    } else {                        // 0 ～ 9 の範囲外の値が指定された場合は７セグメント LED を全消灯する
        temp_gpio = 0xFF00;
        //y GPIOB->ODR &= ~temp_gpio;
        GPIOB->BRR = temp_gpio;
    }
}
#endif

#if 1
/*
 * 最初のコード
 *
 *
 */
void Display_7Seg(int number) {
    /*
    **      < 7 セグメント LED のポート割り当て >
    **
    **             a        a - PB8
    **           ----        b - PB9
    **        f / g / b        c - PB10
    **          ----        d - PB11
    **       e /   / c        e - PB12
    **         ----            f - PB13
    **          d            g - PB14
    */

    /* 7 セグメント LED 表示パターン用のデータ配列（正論理として定義） */
    const static int Seg7led_Table[] = { 0x3f00,    // 0
                                         0x0600,    // 1
                                         0x5b00,    // 2
                                         0x4f00,    // 3
                                         0x6600,    // 4
                                         0x6d00,    // 5
                                         0x7d00,    // 6
                                         0x2700,    // 7
                                         0x7f00,    // 8
                                         0x6f00     // 9
    };

    int temp_gpio;

    if ((number >= 0) && (number <= 9)) {                // 数値が 0 ～ 9 の範囲なら指定された数字のパターンを
        temp_gpio = Seg7led_Table[number] & 0x7f00;

        GPIOB->ODR = (GPIOB->ODR & 0x00FF) | temp_gpio;  // 下位8ビットを保護しつつ上位8ビットを設定
    } else {                        // 0 ～ 9 の範囲外の値が指定された場合は７セグメント LED を全消灯する
        temp_gpio = 0xFF00;
        GPIOB->ODR &= ~temp_gpio;
    }
}
#endif



#if 0
/*
 * ＬＣＤのＲＳとＥＮに影響を与えない様にしたつもりのコード。ただし、ＬＥＤマトリクスへの表示はうまくいかなくなった。
 *
 *　LED状の表示は不具合なるも、LCDには影響がない。
 */
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

	const static int Seg7led_Table[] = { 0x3f00,	// 0
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

	int temp_gpio;

	if ((number >= 0) && (number <= 9)) {				// 数値が 0 ～ 9 の範囲なら指定された数字のパターンを
		temp_gpio= Seg7led_Table[number] & 0x7f00;

		GPIOB->ODR |= temp_gpio;	// 7 セグメント LED に表示する
		GPIOB->ODR &= ~temp_gpio;
	} else {						// 0 ～ 9 の範囲外の値が指定された場合は７セグメント LED を全消灯する
		temp_gpio=0xFF00;
		GPIOB->ODR &= ~temp_gpio;
	}
}
#endif






/*
 * https://nicerland.com/stm32f103/
 * のコードを転用
 */
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


/*
 * https://nicerland.com/stm32f103/
 * のコードを転用
 */
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

#if 0
//以下不具合の歴史を記録

	//以下のロジック、スイッチの押下を検出したときの、前回の確定値の扱い、
	//立上りフラグの扱いは苦労した。2024/05/16 23:00
	//いまのところ、同一キー入力が受け入れられない状況。

//	//直前までの確定値と直近の確定値が等しくなければ、
//	//現時点での確定値が０以外ならば、意味のあるキーの押下があったものとして、
//	//押下フラグと、前回の確定値とを更新する。
//	if((determined_key_code!=last_determined_key_code)){
//		if((last_determined_key_code==0)&&(determined_key_code!=0)){
//			key_pushed_flag=PUSHED;
//			last_determined_key_code=determined_key_code;
//		}
//	}

//	if(determined_key_code != 0){
//		key_pushed_flag=PUSHED;
//		last_determined_key_code=determined_key_code;
//	}
#endif

	//のたうち回って、ようやくできた。2024/05/18 01:38
	if((last_determined_key_code==0)&&(determined_key_code!=0)){
		key_pushed_flag=PUSHED;
		keypad_clack_sound();
	}

	last_determined_key_code=determined_key_code;

	return;
}



void wait_any_key_input(void)
{
	while(key_pushed_flag!=PUSHED);	//キー入力があるまで待つ。
	key_pushed_flag=0;				//キー入力があったので、押下フラグをクリアする。
	keypad_clack_sound();
}


void begin_keyin_prompt(void)
{
	lcd_all_clear();
	lcd_gotoxy(1,1);
	lcd_print("Hit * to initialize.");
}

void numbers_key_prompt(void)
{
	//2024年7月5日朝　強制初期化
	lcd_init();
	//lcd_clear_line(1);
	lcd_gotoxy(1,1);
	lcd_print("Num(s), then #.");
}



/*
 * キーパッドから数値を取り込む
 *
 */
#define INITIAL		0
#define INPROGRESS	20
#define	POSTPROCESS	30
#define ERROR		40
#define	COMPLETE	50



#if 1
int get_number(int *number)
{
	int digit;						//入力した数値の桁数
	char number_string[MAX_NUM];	//入力した数値をLEDマトリクスに表示するための変数

	QUEUE_T queue;					//数字キーの打鍵順に保存するキュー

	int state=INITIAL;
	begin_keyin_prompt();

	while(state!=COMPLETE){

		while(key_pushed_flag!=PUSHED);	//キー入力があるまで待つ。
		key_pushed_flag=0;	//キー入力があったので、押下フラグをクリアする。

		keypad_clack_sound();
//		melody_address=melody[4];	//＊＊＊＊妥協的打鍵音
//		play_music(melody_address);	//

		switch(state){

		case INITIAL:


			if(determined_key_code=='*'){
				initQueue(&queue);
				digit=0;
				numbers_key_prompt();

#ifdef DISP_7SEG
			enable_segment_display=ENABLE;	//7セグＬＥＤに入力値を表示する
			memset(num_on_7segLED,0xff,sizeof(num_on_7segLED));
#endif



				state=INPROGRESS;
			}
			break;

		case INPROGRESS:

			if(determined_key_code=='#'){//#キー押下で、キー入力終了
					state=COMPLETE;
			}else if(determined_key_code!='*'){//*キー以外（数字キー）押下時の処理

				enqueue(&queue, determined_key_code);	//打鍵したキーコードをキューに格納
				digit=QueueToString(&queue, number_string);//打鍵した桁数を取得

				Queue_to_number_array(&queue, num_on_7segLED);//キューの内容を7セグＬＥＤ表示用配列にコピー

				lcd_clear_line(2);	lcd_gotoxy(1,2);	//LCDに値を表示
				lcd_print(number_string);



			}
			break;
		}
	}

#ifdef DISP_7SEG
	enable_segment_display=DISABLE;	//7セグＬＥＤの表示を止める。
	SelectDigit(-1);	//7セグＬＥＤを明示的に消灯する
#endif

	sscanf(number_string,"%d",number);//数字列を数値に変換、get_numberの引数として呼び出し側へ渡す。

	return digit;


}

#endif

#if 0
int get_number(int *number){

	int digit;
	char number_string[MAX_NUM];

	QUEUE_T queue;

	int state=INITIAL;

	while(state!=COMPLETE){

		while(key_pushed_flag!=PUSHED);	//キー入力があるまで待つ。
		key_pushed_flag=0;	//キー入力があったので、押下フラグをクリアする。

		keypad_clack_sound();
//		melody_address=melody[4];	//＊＊＊＊妥協的打鍵音
//		play_music(melody_address);	//

		switch(determined_key_code){
			case '*':					//キューの初期化
				initQueue(&queue);
				numbers_key_prompt();
				state=INPROGRESS;
				break;
			case '#':
				if(state!=INPROGRESS){
					break;
				}else{
					//printf("キューの中身を表示\n");
					//printQueue(&queue);
					state=COMPLETE;
					enable_segment_display=DISABLE;	//7セグＬＥＤの表示を止める。
					SelectDigit(-1);	//7セグＬＥＤを明示的に消灯する
					break;
				}

			default://数字キーの場合
				if(state!=INPROGRESS){
					//printf("Que is not ready. Hit # to initialize the que.\n");
					break;
				}else{
					//printf("[num=%d]\n",num);
					enable_segment_display=ENABLE;	//7セグＬＥＤに入力値を表示する

					enqueue(&queue, determined_key_code);
					Queue_to_number_array(&queue, num_on_7segLED);
					break;
				}
		}
	}

	digit=QueueToString(&queue, number_string);
	sscanf(number_string,"%d",number);
	return digit;

}
#endif


/*
 * chatGPTにより生成した関数
 * 値numberを与えると、その値を配列num_digitに一桁ずつ格納する。
 * 8桁に満たない場合は、ゼロ詰めをせずに、7セグメントＬＥＤに表示しえない99で詰める。
 *
 */
//void intToDigits(int number, int num_digit[8]) {
//    for (int i = 0; i < 8; i++) {
//        if (number == 0) {
//            num_digit[i] = 99; // numberが0になったら残りは99で埋める
//        } else {
//            num_digit[i] = number % 10; // 1の位の数字を取得
//            number /= 10; // 次の位に移動
//        }
//    }
//}

void intToDigits(int number, int num_digit[8]) {
    for (int i = 0; i < 8; i++) {
        if (number == 0) {
            num_digit[i] = 99; // numberが0になったら残りは99で埋める(0-9以外という意味）
        } else {
            num_digit[i] = (number % 10); // 1の位の数字を取得
            number /= 10; // 次の位に移動
        }
    }
}




void Queue_to_number_array(QUEUE_T* queue, int num[])
{
	int digit;
	int number;
	char number_string[MAX_NUM];


	digit=QueueToString(queue,number_string);
	number=atoi(number_string);

	intToDigits(number, num);

}


/*
 *



 *
*/






int Mister_Collatz(int start_number)
{
	int count=0;
	int val=start_number;
	char number_string[MAX_NUM];	//LCDに表示するための数値の数字列
	char collatz_result_string[16+1];

	sprintf(number_string,"%d", start_number);

	lcd_clear_line(1);
	lcd_gotoxy(1,1);
	lcd_print("StartFrom ");
	lcd_print(number_string);

#ifdef DISP_7SEG
	enable_segment_display=ENABLE;	//7セグＬＥＤに計算過程を表示する
#endif

	while(val!=1){
		if(val%2==0)
			val=val/2;
		else
			val=3*val+1;
		count++;

		//計算過程のＬＣＤ上の表示
		sprintf(collatz_result_string,"[%d]-[%d]",count, val);
		//2024年7月5日　強制初期化
		lcd_all_clear();

		lcd_clear_line(2);
		lcd_gotoxy(1,2);
		lcd_print(collatz_result_string);

		//計算過程の7セグＬＥＤ上の表示
		intToDigits(val, num_on_7segLED);

		//計算過程のサウンドを鳴動
		inProgress_sound();

		Wait_Time(300);
	}

#ifdef DISP_7SEG
	enable_segment_display=DISABLE;	//7セグＬＥＤに計算過程の表示を止める
#endif

	return count;
}




void Disp_number_on_7_segment_LED(void){


	static int digit_index=0;		//7セグへの表示桁、1の位から表示

	if(digit_index==8){
		digit_index=0;
		SelectDigit(-1);
	}else{
		SelectDigit(digit_index);
		Display_7Seg(num_on_7segLED[digit_index]);
		digit_index++;	//次の桁の表示準備
	}
	return ;

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

	if(enable_segment_display==ENABLE){
		Disp_number_on_7_segment_LED();
	}

}

#if 1

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

#endif

