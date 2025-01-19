/*
 * 20240305-Program9-3.c
 *
 *  Created on: 2024/03/05
 *      Author: shidou01
 */


/*
	動かない　２０２４・０３・０５

	E	PA5	CN10-11
	R/W	GND	OnBoardGround-Connected
	RS	PA4	CN7-32

	D7	PA3	CN10-37
	D6	PA2	CN10-35
	D5	PA1	CN7-30
	D4	PA0	CN7-28



	2024/03/06
	やたらと、delayを入れた。
	山田先生のプログラムを動かした後（LCD表示OK）、LCDの電源を切らずに配線をし直して、
	このプログラムを動作させると、文字化けしたHelloWorldが表示された。

	初期化がうまくいっていない様子。


	2024/03/07
	相変わらず動かない。

	2024/03/08
	このプログラムの初期化コマンドのシーケンスを山田先生のプログラムで検証したところ、正常動作することを確認。
	ただし、このプログラムでは相変わらず動作はしない。

	配線ミスを疑うも、間違いなし。

	ポートに信号が出ているのかという点を疑い、LEDの点灯で、ｒｓの動作を確認。
	コマンドの場合はL、文字データの場合はHであることを確認。
	Eは、オンボードのLEDの点灯で確認。
	ところが、データ線を調べていて、D7-PA2、D6-PA3は、デバッガからの制御が効かないを確認。
	これらの線は仮想シリアルポートであることを思い出して、いろいろなことが氷解。
	ポートのビット割り当てを変えなければならない。本の通りには動作しないということ。
	この本の使い方として、注意を要する。


*/


#include <stm32f10x.h>

void lcd_init(void);
void lcd_sendCommand(unsigned char cmd);
void lcd_sendData(unsigned char data);
void lcd_print(char * str);

void delay_us(uint16_t t);

#define LCD_RS	4
#define LCD_EN	5

#define DELAY	20000

int main()
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;	// GPIOAのクロックを有効にする

//	RCC->APB2ENR|=0xFC;	//Enable clock for GPIO
//				 76543210
	GPIOA->CRL=0x44333333;	//PA0-PA5 as outputs original　汎用出力50MHｚ*/	//本質ではない、はず。。。
//	GPIOA->CRL=0x44111111;	//PA0-PA5 as outputs　汎用出力10MＨｚ*/
//	GPIOA->CRL=0x44222222;	//PA0-PA5 as outputs yamada sensei 汎用出力2ＭＨｚ*/



	lcd_init();

	lcd_print("Hello world!");
	while(1);


}

void lcd_putValue1(unsigned char value)
{
	GPIOA->BRR=0x0f;	//clear pa0-pa3
	GPIOA->BSRR=value&0x0f;	//put low nibble on pa0-pa3
	delay_us(DELAY);

	GPIOA->ODR |= (1<<LCD_EN);	// EN=1 for H-to-L pulse
	delay_us(DELAY);
	GPIOA->ODR &= ~(1<<LCD_EN);	// EN=0 for H-toL pulse
	delay_us(DELAY);
}

void lcd_putValue(unsigned char value)
{
	GPIOA->BRR=0x0f;	//clear pa0-pa3
	GPIOA->BSRR=(value>>4)&0x0f;	//put high nibble on pa0-pa3
	delay_us(DELAY);

	GPIOA->ODR |= (1<<LCD_EN);	// EN=1 for H-to-L pulse
	delay_us(DELAY);
	GPIOA->ODR &= ~(1<<LCD_EN);	// EN=0 for H-toL pulse
	delay_us(DELAY);

	GPIOA->BRR=0x0f;	//clear PA0-PA3
	GPIOA->BSRR=value&0x0f;	//put low nibble on pa0-pa3

	GPIOA->ODR |=(1<<LCD_EN);	//EN=1 for H-toL pulse
	delay_us(DELAY);
	GPIOA->ODR &= ~(1<<LCD_EN);	//EN=0 for H-to-L pulse
	delay_us(DELAY);

}


void lcd_sendCommand1(unsigned char cmd)
{
	GPIOA->ODR &= ~(1<<LCD_RS);	//RS =0 for command
	delay_us(DELAY);

	lcd_putValue1(cmd);


}

void lcd_sendCommand(unsigned char cmd)
{
	GPIOA->ODR &= ~(1<<LCD_RS);	//RS =0 for command
	delay_us(DELAY);

	lcd_putValue(cmd);

}

void lcd_sendData(unsigned char data)
{
	GPIOA->ODR |=(1<<LCD_RS);	//RS=1 for data
	delay_us(DELAY);
	lcd_putValue(data);
	delay_us(DELAY);

}

void lcd_print(char* str)
{
	unsigned char i=0;
	while(str[i]!=0){
		lcd_sendData(str[i]);
		i++;
	}
}

void lcd_init()
{
//	GPIOA->ODR &=~(1<<LCD_EN);	//LCD_EN=0
	delay_us(DELAY);


	lcd_sendCommand1(0x3);	//8bit mode 7654
							//          0011
	lcd_sendCommand1(0x3);
	lcd_sendCommand1(0x3);
	lcd_sendCommand1(0x2);	//4bit mode 7654
							//          0010

	lcd_sendCommand(0x28);	//functionSet 4bit, 2line, 5x7font
	lcd_sendCommand(0x08);	//dispTurnOff, cursorOff, blinkOff
	lcd_sendCommand(0x01);	//dispClear
	lcd_sendCommand(0x06);	//entryMode cursorMoveRight
	lcd_sendCommand(0x0f);	//dispTurnOn


	//lcd_sendCommand(0x33);	//
	//lcd_sendCommand(0x32);
	//lcd_sendCommand(0x28);	//init lcd 2 line ,5x7 matrix

//	lcd_sendCommand(0x03);	//
//	lcd_sendCommand(0x03);	//
//	lcd_sendCommand(0x03);	//

////	lcd_sendCommand(0x33);	//
//	lcd_sendCommand(0x32);	//
//	lcd_sendCommand(0x28);	//

//	lcd_sendCommand(0x28);	//
//	lcd_sendCommand(0x28);	//
//	lcd_sendCommand(0x80);	//
//	lcd_sendCommand(0x01);	//
//	lcd_sendCommand(0x06);	//
//	lcd_sendCommand(0x0f);	//


	//lcd_sendCommand(0x0e);	//display on, cursor on
	//lcd_sendCommand(0x01);	//clear lcd
	//delay_us(DELAY);
	//lcd_sendCommand(0x06);	//shift cursor right
}

void delay_us(uint16_t t)
{
	volatile unsigned long l=0;
	for(uint16_t i=0; i<t; i++)
		for(l=0; l<6; l++);
}
