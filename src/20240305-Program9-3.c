/*
 * 20240305-Program9-3.c
 *
 *  Created on: 2024/03/05
 *      Author: shidou01
 */


/*
	ìÆÇ©Ç»Ç¢Å@ÇQÇOÇQÇSÅEÇOÇRÅEÇOÇT

	E	PA5	CN10-11
	R/W	GND	OnBoardGround-Connected
	RS	PA4	CN7-32

	D7	PA3	CN10-37
	D6	PA2	CN10-35
	D5	PA1	CN7-30
	D4	PA0	CN7-28






*/


#include <stm32f10x.h>

void lcd_init(void);
void lcd_sendCommand(unsigned char cmd);
void lcd_sendData(unsigned char data);
void lcd_print(char * str);

void delay_us(uint16_t t);

#define LCD_RS	4
#define LCD_EN	5

int main()
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;	// GPIOAÇÃÉNÉçÉbÉNÇóLå¯Ç…Ç∑ÇÈ

//	RCC->APB2ENR|=0xFC;	//Enable clock for GPIO
//				 76543210
	GPIOA->CRL=0x44111111;	//PA0-PA5 as outputs*/

	lcd_init();

	lcd_print("Hello world!");
	while(1);


}



void lcd_putValue(unsigned char value)
{
	GPIOA->BRR=0x0f;	//clear pa0-pa3
	GPIOA->BSRR=(value>>4)&0x0f;	//put high nibble on pa0-pa3

	GPIOA->ODR |= (1<<LCD_EN);	// EN=1 for H-to-L pulse
	delay_us(10);
	GPIOA->ODR &= ~(1<<LCD_EN);	// EN=0 for H-toL pulse
	delay_us(100);

	GPIOA->BRR=0x0f;	//clear PA0-PA3
	GPIOA->BSRR=value&0x0f;	//put low nibble on pa0-pa3

	GPIOA->ODR |=(1<<LCD_EN);	//EN=1 for H-toL pulse
	delay_us(10);
	GPIOA->ODR &= ~(1<<LCD_EN);	//EN=0 for H-to-L pulse
	delay_us(100);

}

void lcd_sendCommand(unsigned char cmd)
{
	GPIOA->ODR &= ~(1<<LCD_RS);	//RS =0 for command
	lcd_putValue(cmd);

}

void lcd_sendData(unsigned char data)
{
	GPIOA->ODR |=(1<<LCD_RS);	//RS=1 for data
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
	delay_us(3000);

	lcd_sendCommand(0x33);	//
	lcd_sendCommand(0x32);
	lcd_sendCommand(0x28);	//init lcd 2 line ,5x7 matrix
	lcd_sendCommand(0x0e);	//display on, cursor on
	lcd_sendCommand(0x01);	//clear lcd
	delay_us(2000);
	lcd_sendCommand(0x06);	//shift cursor right
}

void delay_us(uint16_t t)
{
	volatile unsigned long l=0;
	for(uint16_t i=0; i<t; i++)
		for(l=0; l<6; l++);
}
