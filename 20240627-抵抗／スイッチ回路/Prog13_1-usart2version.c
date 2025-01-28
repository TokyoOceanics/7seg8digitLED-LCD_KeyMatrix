/*

 2024/06/16

 This program configures PA1(ADC_IN1) as the ADC input and sends
the result through usart2

 PA1(ADC1-IN1)�̃A�i���O���͂��t�r�`�q�s�Q��TX�ɑ���v���O����

 Nicerland��prog13-1�ƁA���a�搶��Usart�v���O�����Ƃ����́B

�@ADC1-IN1 PA1(CN7-30)
 USART2 TX PA2(USB���z�V���A���|�[�g)
 USART2 RX PA3(USB���z�V���A���|�[�g)

*/

#include <stm32f10x.h>
#include <stdio.h>

void usart2_init(void);
void usart2_sendByte(unsigned char c);
void usart2_sendStr(char *str);
void usart2_sendInt(unsigned int i);

void delay_ms(uint16_t t);
void delay_us(uint16_t t);



void calibrate_AD1(void);

void ADC_Calibration(ADC_TypeDef* ADCx);


int main()
{
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; //USRT2 �C�l�[�u��

	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN |RCC_APB2ENR_ADC1EN; //�|�[�gA���C�l�[�u��
	GPIOA->CRL = 0x44444904;	//PA3=AFIO_in(usrt2_rx) PA2=AFIO_out(uart2_tx)
								//PA1(ADC1_IN1) analog input



	ADC1->CR2 = 1;	/* ADON = 1 (power-up) */

	//calibrate_AD1();
	ADC_Calibration(ADC1);

	ADC1->SMPR2 = 1<<3; /* SMP1 = 001 */
		
	delay_us(1);	/* wait 1us to make sure the adc module is stable */
	
	usart2_init(); /* initialize the usart1 */
	
	while(1)
	{
		ADC1->SQR3 = 1;	/* choose channel 1 as the input */
		ADC1->CR2 = 1;	/* ADON = 1 (start conversion) */
		while((ADC1->SR&(1<<1)) == 0); /* wait until the EOC flag is set */
		
		usart2_sendInt(ADC1->DR);	/* read ADC1_DR and send its value through serial */
		usart2_sendStr("\n\r");		/* go to new line */

		delay_ms(1000);		/* wait 10ms */
	}
}


//�`�c�R���o�[�^�̍Z������
//
void calibrate_AD1(void){
	ADC1->CR2 = 1;	/* ADON = 1 (power-up) */
	ADC1->CR2|=(1<<2);	//ADC_CAL�t���O�ōZ��
	while((ADC1->CR2&(1<<2))!=0);	//�I���҂�
}

/**
 * @brief  �w�肵��ADC�̍Z�����s���܂��B
 * @param  ADCx: x��1�ŁA�I������ADC�y���t�F�����������܂��B
 * @retval �Ȃ�
 *
 * ���̊֐��́A�w�肵��ADC�̍Z�����W�X�^�����Z�b�g���A
 * ���Z�b�g����������܂őҋ@���܂��B���̌�A�Z�����J�n���A
 * �Z������������܂őҋ@���܂��B
 *
 * �b�������f�o�s�ɂ��Z���֐�
 *
 */
void ADC_Calibration(ADC_TypeDef* ADCx) {
    // �Z�����W�X�^�̃��Z�b�g
    ADCx->CR2 |= ADC_CR2_RSTCAL;
    // ���Z�b�g����������܂ő҂�
    while (ADCx->CR2 & ADC_CR2_RSTCAL);

    // �Z���̊J�n
    ADCx->CR2 |= ADC_CR2_CAL;
    // �Z������������܂ő҂�
    while (ADCx->CR2 & ADC_CR2_CAL);
}




void usart2_init()
{
	//USART2 init.
	USART2->CR1 = 0x200C;
	USART2->BRR = 0x116; // �{�[���[�g�ݒ� 278=36,000,000Hz/115200   278->0x116
	//APB1�̃N���b�N
}

void usart2_sendByte(unsigned char c)
{	
	while((USART2->SR&(1<<6)) == 0);  //wait until the TC flag is set
	USART2->DR = c;
}

/* the function sends a zero ending string through USART1 */
void usart2_sendStr(char *str)
{
	while(*str != 0)
	{
		usart2_sendByte(*str);
		str++;
	}
}

/* The function sends a number through USART1 */
void usart2_sendInt(unsigned int i)
{
	char str[10];
	sprintf(str,"%d",i);
	
	usart2_sendStr(str);
}

void delay_ms(uint16_t t)
{
	for(int i = 0; i < t; i++)
	{
		for(volatile uint16_t a = 0; a < 6000; a++)
		{}
	}
}

void delay_us(uint16_t t)
{
	for(int i = 0; i < t; i++)
	{
		for(volatile uint16_t a = 0; a < 6; a++)
		{}
	}
}
