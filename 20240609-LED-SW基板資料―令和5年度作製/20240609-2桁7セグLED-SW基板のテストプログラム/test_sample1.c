/*
 * 	test sample �s��o�[�W�����@SysTickHander x  SysTick_Handler OK
 */

/*
	�{�[�h��LED�ƃX�C�b�`�̐ڑ�
 	A552-SRD�i2����7�Z�OLED�\����j
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

	�����{�^���X�C�b�`
 	SW1		PC2
 	SW2		PC3

 */



//#include <stddef.h>
#include "stm32f10x.h"

#define CLK	8000000
#define sw1	0x100
#define sw2	0x200

int seg7[]={0xc0, 0xf9, 0xa4,0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90};


int up=0;
int sw_now=0;

void init_system(void);

//void SysTickHandler(void);

void my_delay(int n);




int main(void)
{

	init_system();
//	int count=0;

	unsigned short data;

	GPIOB->ODR=0x0;


	while(1){
		for(int i=0; i<10; i++){
			GPIOA->ODR=seg7[i]<<4;
			my_delay(999999);



		}



	}
}



void init_system(void)
{
	/* �N���b�N���擾�^�ݒ�ׂ̈̍\���̒�` */
	RCC_ClocksTypeDef RCC_Clocks;

	RCC->APB2ENR = RCC_APB2ENR_IOPAEN |	// GPIOA �̃N���b�N��L���ɂ���
				   RCC_APB2ENR_IOPBEN |	// GPIOB �̃N���b�N��L���ɂ���
				   RCC_APB2ENR_IOPCEN;	// GPIOC �̃N���b�N��L���ɂ���


//               FEDCBA98
	GPIOA->CRH=0x44441111;
//               76543210
	GPIOA->CRL=0x11114444;

//               76543210
	GPIOB->CRL=0x41144444;

//               76543210
	GPIOC->CRL=0x44444444;

	/* �V�X�e���̃N���b�N���̎擾 */
	RCC_GetClocksFreq(&RCC_Clocks);

	/* SysTick �^�C�}�̐ݒ� */
	//SysTick_Config(RCC_Clocks.HCLK_Frequency / 2000);	// SysTick �^�C�}�����݂̊Ԋu�� 500us �ɐݒ�

	my_delay(99999);


}







void SysTickHandler(void)
{
	my_delay(9999);
}



