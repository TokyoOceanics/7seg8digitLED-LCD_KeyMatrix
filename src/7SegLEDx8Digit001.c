/*
 * 7SegLEDx8Digit001.c
 *
 *  Created on: 2024/03/04
 *      Author: shidou01
 *
 *      �͌��搶�̕\����H���p��̓���m�F�@7�Z�OLED�~8������
 *
 */


/*
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


#include "main.h"

#define UserButtonSW	(GPIOC->IDR)&(0x01<<13)



//�O���[�o���ϐ�
int TimeInt=0;	//delay�����iWait����Ԋu���w�肷�邽�߁j�̃O���[�o���ϐ��iSysTick�Ō��Z�j




//�֐��̃v���g�^�C�v�錾
void my_init_system(void);
void Wait_Time(int time);	//SysTick_Handler���g���҂����ԏ���

void SelectDigit(int digit);
void Display_7Seg(int number);



//
//���C���֐�
//
int main(void)
{

	my_init_system();//�n�[�h�E�F�A�̏�����

	while(1){
		for(int digit=0; digit<=7; digit++){
			SelectDigit(digit);
			for(int data=0; data<10; data++){
				Display_7Seg(data);
				Wait_Time(50);
			}

		}

	}
}




//
//�n�[�h�E�F�A�̏�����
//

void my_init_system(void)
{

	/* �N���b�N���擾�^�ݒ�ׂ̈̍\���̒�` */
	RCC_ClocksTypeDef RCC_Clocks;

	/* �V�X�e���̃N���b�N���̎擾 */
	RCC_GetClocksFreq(&RCC_Clocks);

	/* SysTick �^�C�}�̐ݒ� */
	/*SysTick �^�C�}�����݂̊Ԋu�� 1ms �ɐݒ� */
	// ���荞�݊Ԋu��SysTick_Config()�̈�����RCC_Clocks.HCLK_Frequency / 1000�@�Ƃ��āA1msec
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);	//

	//GPIO�@A��B��L����
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN|RCC_APB2ENR_IOPCEN;	// GPIOB,C�̃N���b�N��L���ɂ���

	//TIM3 �L�������āAPB0��TIM3�Ƃ���
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;	//TIM3�̗L����

	//�|�[�gB�̓��o�͐ݒ�@GPIOB->CRL�ݒ�́A�|�[�gB�̓��o�͐ݒ�@pb0��TIM3�Ƃ���
	//PB0�@CNF=10�AMODE=11�́@�I���^�l�[�g�o�́E�v�b�V���v��
	//			 76543210
	GPIOB->CRL=0x4444444B;
	//			 FEDCBA98
	GPIOB->CRH=0x11111111;

	//�|�[�gC
	//			 76543210
	GPIOC->CRL=0x11111111;
	//			 FEDCBA98
	GPIOC->CRH=0x11141111;



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
**      < 7 �Z�O�����g LED �̃|�[�g���蓖�� >
**
**             a		a - PB8
**           ----		b - PB9
**        f / g / b		c - PB10
**          ----		d - PB11
**       e /   / c		e - PB12
**         ----			f - PB13
**          d			g - PB14
*/

/* 7 �Z�O�����g LED �\���p�^�[���p�̃f�[�^�z��i���_���Ƃ��Ē�`�j */

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

	if ((number >= 0) && (number <= 9)) {				// ���l�� 0 �` 9 �͈̔͂Ȃ�w�肳�ꂽ�����̃p�^�[����
		GPIOB->ODR = Seg7led_Table[number] & 0x7f00;	// 7 �Z�O�����g LED �ɕ\������i���_���̈׃r�b�g���]�j
	} else {											// 0 �` 9 �͈̔͊O�̒l���w�肳�ꂽ�ꍇ��
		GPIOB->ODR = 0x7f00;							// �V�Z�O�����g LED ��S��������
	}
}






/*
 * �҂����Ԋ֐��@�iSysTick_Handler�ƘA�g�j
 */

void Wait_Time(int time)
{
	TimeInt=time;
	while(TimeInt>0);	//�J�E���^�̒l��0�ɂȂ�܂ő҂B�J�E���^��SysTick_Handler�Ō��Z����Ă���B


}





/*
 * SysTick_Handler�֐�
*/

void SysTick_Handler(void)
{





	//
	//�҂����Ԋ֐��@Wait_Time()�p�̕ϐ��̏���
	//
	if(TimeInt>0) TimeInt--;





}
