/*
 * 20240310-Program9-4a.c
 *
 *  Created on: 2024/03/10
 *      Author: x-lib
 *
 *  The STM32F103 Arm Microcontroller and Embedded Systems: Using Assembly and C
 *  Program9-4.c をSTM32 Nucleo　F103RBにポーティング
 *
 *  河原先生製作の7セグメントLED/LCD表示回路に文字列を表示するプログラム
 *
 *
 */

/*
 *
 * 	D7	PA7	CN10-15
 * 	D6	PA6	CN10-13
 * 	D5	PA5	CN10-11
 * 	D4	PA4	CN7-32
 *
 * 	RS	PA1	CN7-30
 * 	E	PA0	CN7-28
 *
 *
 */


#include <stm32f10x.h>

void lcd_init(void);
void lcd_sendCommand(unsigned char cmd);
void lcd_sendData(unsigned char data);
void lcd_print(char * str);
void lcd_gotoxy(unsigned char x, unsigned char y);

void delay_us(uint16_t t);

void system_init(void);


#define LCD_RS	1
#define LCD_EN	0

#define DELAY	1000

int main()
{

	system_init();
	lcd_init();


	lcd_gotoxy(1,1);				//move cursor to 1,1
	lcd_print("The world is but");	//show "The world is but"
	lcd_gotoxy(1,2);				//move corsor to 1,2
	lcd_print("one country");		//show "one country"

	while(1);						//stay here forever

}


void lcd_putValue(unsigned char value)
{
	//valueの上位4ビットをPA7-PA4（LCDのD7-D4）に出力する
	GPIOA->BRR=0b11110000;			//clear PA7-PA4
	GPIOA->BSRR=value&0b11110000;	//put high nibble on pa7-pa4
	delay_us(DELAY);

	//ストローブ信号（Eの立下り）で、LCDのレジスタにデータを転送
	GPIOA->ODR |= (1<<LCD_EN);	// EN=1 for H-to-L pulse
	delay_us(DELAY);
	GPIOA->ODR &= ~(1<<LCD_EN);	// EN=0 for H-toL pulse
	delay_us(DELAY);

	//valueの下位4ビットデータをPA7-PA4（LCDのD7-D4）に出力する
	GPIOA->BRR=0b11110000;			//clear PA7-PA4
	GPIOA->BSRR=(value<<4)&0xf0;	//put low nibble on pa7-pa4

	//ストローブ信号（Eの立下り）で、LCDのレジスタにデータを転送
	GPIOA->ODR |=(1<<LCD_EN);	//EN=1 for H-toL pulse
	delay_us(DELAY);
	GPIOA->ODR &= ~(1<<LCD_EN);	//EN=0 for H-to-L pulse
	delay_us(DELAY);

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
	GPIOA->ODR &=~(1<<LCD_EN);	//LCD_EN=0
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

void delay_us(uint16_t t)
{
	volatile unsigned long l=0;
	for(uint16_t i=0; i<t; i++)
		for(l=0; l<6; l++);
}


/*カーソルを指定位置に移動する*/
void lcd_gotoxy(unsigned char x, unsigned char y)
{
	const unsigned char firstCharAdr[]={0x80,0xc0, 0x94,0xd4};
	lcd_sendCommand(firstCharAdr[y-1]+x-1);
	delay_us(100);
}

void system_init(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;	// GPIOAのクロックを有効にする

	//			 	 76543210
	//	GPIOA->CRL=0x33334433;	//PA7-PA4, PA1,PA0 as outputs original　汎用出力50MHｚ*/	//本質ではない、はず。。。

		GPIOA->CRL=0x11114411;	//汎用出力10MＨｚ*/
	//	GPIOA->CRL=0x22224422;	//yamada sensei 汎用出力2ＭＨｚ*/

}

