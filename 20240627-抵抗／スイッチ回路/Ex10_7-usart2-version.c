/*
 *
 2024/06/16

 Nicerland��Ex10_7.c�ƁA���a�搶��Usart�v���O�����Ƃ����́B

�@USART2 TX PA2(USB���z�V���A���|�[�g)
 USART2 RX PA3(USB���z�V���A���|�[�g)

 ��������t�r�`�q�s�Q�o�R�ő��o����v���O����


 *
 *
 */



#include "stm32f10x.h"

void delay_ms(uint16_t t);
void usart2_sendByte(unsigned char c);

int main()
{
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

	GPIOA->CRL = 0x44444944;	//PA3=AFIO_in(usrt2_rx) PA2=AFIO_out(uart2_tx)

	//USART2->BRR = 0xD06; // �{�[���[�g�ݒ� 9600=32,000,000Hz/3334   3334->0xD06
	USART2->BRR = 0x116; // �{�[���[�g�ݒ� 278=32,000,000Hz/115200   278->0x116
	USART2->CR1 = 0x200c; //UE=1,TE=1,RE=1




	
	while(1)
	{		
		usart2_sendByte('H');
		usart2_sendByte('i');
		usart2_sendByte('\n'); //go to new line
		usart2_sendByte('\r'); //carrier return
		
		delay_ms(1000);	//wait 1 second
	}
}

void usart2_sendByte(unsigned char c)
{
	USART2->DR = c;
	

//	delay_ms(1000);	//wait 1 second
	while((USART2->SR&(1<<6)) == 0);  //wait until the TC flag is set
	USART2->SR &= ~(1<<6);
}

void delay_ms(uint16_t t)
{
	volatile unsigned long l = 0;
	for(uint16_t i = 0; i < t; i++)
		for(l = 0; l < 6000; l++)
		{
		}
}
