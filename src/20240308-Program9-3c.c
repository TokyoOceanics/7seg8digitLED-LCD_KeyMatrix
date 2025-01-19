/*
 * 20240305-Program9-3.c
 *
 *  Created on: 2024/03/05
 *      Author: shidou01
 */


/*
	�����Ȃ��@�Q�O�Q�S�E�O�R�E�O�T

	E	PA5	CN10-11
	R/W	GND	OnBoardGround-Connected
	RS	PA4	CN7-32

	D7	PA3	CN10-37
	D6	PA2	CN10-35
	D5	PA1	CN7-30
	D4	PA0	CN7-28



	2024/03/06
	�₽��ƁAdelay����ꂽ�B
	�R�c�搶�̃v���O�����𓮂�������iLCD�\��OK�j�ALCD�̓d����؂炸�ɔz�����������āA
	���̃v���O�����𓮍삳����ƁA������������HelloWorld���\�����ꂽ�B

	�����������܂������Ă��Ȃ��l�q�B


	2024/03/07
	���ς�炸�����Ȃ��B

	2024/03/08
	���̃v���O�����̏������R�}���h�̃V�[�P���X���R�c�搶�̃v���O�����Ō��؂����Ƃ���A���퓮�삷�邱�Ƃ��m�F�B
	�������A���̃v���O�����ł͑��ς�炸����͂��Ȃ��B

	�z���~�X���^�����A�ԈႢ�Ȃ��B

	�|�[�g�ɐM�����o�Ă���̂��Ƃ����_���^���ALED�̓_���ŁA�����̓�����m�F�B
	�R�}���h�̏ꍇ��L�A�����f�[�^�̏ꍇ��H�ł��邱�Ƃ��m�F�B
	E�́A�I���{�[�h��LED�̓_���Ŋm�F�B
	�Ƃ��낪�A�f�[�^���𒲂ׂĂ��āAD7-PA2�AD6-PA3�́A�f�o�b�K����̐��䂪�����Ȃ����m�F�B
	�����̐��͉��z�V���A���|�[�g�ł��邱�Ƃ��v���o���āA���낢��Ȃ��Ƃ��X���B
	�|�[�g�̃r�b�g���蓖�Ă�ς��Ȃ���΂Ȃ�Ȃ��B�{�̒ʂ�ɂ͓��삵�Ȃ��Ƃ������ƁB
	���̖{�̎g�����Ƃ��āA���ӂ�v����B


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
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;	// GPIOA�̃N���b�N��L���ɂ���

//	RCC->APB2ENR|=0xFC;	//Enable clock for GPIO
//				 76543210
	GPIOA->CRL=0x44333333;	//PA0-PA5 as outputs original�@�ėp�o��50MH��*/	//�{���ł͂Ȃ��A�͂��B�B�B
//	GPIOA->CRL=0x44111111;	//PA0-PA5 as outputs�@�ėp�o��10M�g��*/
//	GPIOA->CRL=0x44222222;	//PA0-PA5 as outputs yamada sensei �ėp�o��2�l�g��*/



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
