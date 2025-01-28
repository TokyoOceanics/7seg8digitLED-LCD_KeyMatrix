/*
 * 	2����7�Z�O�ɁA�����͌�1�A��͌��Q�ɁA���ꂼ����݂ɐ��l��\������B
 * 	SysTick_Handler�̂Â�𐳊m�ɂ��邱�ƁB
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
 	SW1		PC0
 	SW2		PC1

 */



//#include <stddef.h>
#include "stm32f10x.h"

//�VsegLED�̕\���f�[�^�A���_��
int seg7[]={0xc0, 0xf9, 0xa4,0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90};

//7segLED�̕\�����I���A���_��
//             76543210
int digit[]={0b11011111,0b10111111};


void init_system(void);


void my_delay(int n);//���[�v�����ɂ��҂����Ԑ����֐�
void Wait_Time(int);//SysTick_Handler���g���҂����Ԑ����֐�


int TimeInt=0;	//Wait����Ԋu���w�肷�邽�߂̃O���[�o���ϐ��iSysTick�Ō��Z�j

#define Delay_Time	10	//Wait���ԁ@1ms�P��


int main(void)
{

	init_system();


//	while(1){
//		for(int n=0; n<10; n++)
//			for(int i=n; i<n+2; i++){
//				GPIOB->ODR=digit[i%2];	//7seg�̕\������؂�ւ���
//				GPIOA->ODR=seg7[i]<<4;	//7seg�ɐ�����\��
//				Wait_Time(Delay_Time);
//			}
//	}

	while(1){
			for(int i=0; i<2; i++){
				GPIOB->ODR=digit[i%2];	//7seg�̕\������؂�ւ���
				GPIOA->ODR=seg7[i]<<4;	//7seg�ɐ�����\��
				Wait_Time(Delay_Time);
			}
		}


}



void init_system(void)
{
	/* �N���b�N���擾�^�ݒ�ׂ̈̍\���̒�` */
	RCC_ClocksTypeDef RCC_Clocks;


	/* �V�X�e���̃N���b�N���̎擾 */
	RCC_GetClocksFreq(&RCC_Clocks);

	/* SysTick �^�C�}�̐ݒ� */
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);	// SysTick �^�C�}�����݂̊Ԋu�� 1ms �ɐݒ�



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
}







void SysTick_Handler(void)
{
	//���荞�݊Ԋu�P�����A1��̃_�E���J�E���g�ɂP����
	if(TimeInt>0) TimeInt--;


}



