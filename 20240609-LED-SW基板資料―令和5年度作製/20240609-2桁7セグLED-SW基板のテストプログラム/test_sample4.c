/*
 * 	00����FF�܂Ő��l��\������B
 * 	�u�����N���Ԃ�݂��āA�\�������₷������B
 *
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

int seven_segment_led[3];


//7segLED�̕\�����I���A���_��
//             76543210   76543210   76543210
int digit[]={0b11011111,0b10111111,0b11111111};
//           1���ڂ̕\��    2���ڂ̕\��   �S����


void my_init_system(void);


void my_delay(int n);//���[�v�����ɂ��҂����Ԑ����֐�
void Wait_Time(int);//SysTick_Handler���g���҂����Ԑ����֐�


int TimeInt=0;	//Wait����Ԋu���w�肷�邽�߂̃O���[�o���ϐ��iSysTick�Ō��Z�j

#define Delay_Time	10	//Wait���ԁ@1ms�P��


int set_segment(int value);
int num_to_2digit(int num,int *digit_one, int *digit_ten);


int main(void)
{

	my_init_system();



	while(1){
		for(int i=0; i<100; i++){
			set_segment(i);
			Wait_Time(1000);
		}
	}

}



void my_init_system(void)
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


//2���̐��l����1�̈ʁA10�̈ʂ��Ƃ̐��l�����߂�B
int num_to_2digit(int num,int *digit_one, int *digit_ten)
{
	//�l�����̏ꍇ�ƁA100�𒴂���ꍇ�̓G���[�Ƃ���B
	if((num>100)||(num<0)){
		*digit_one=0;
		*digit_ten=0;
		return -1;
	}

	//0�`99�Ȃ琳�퓮��
	*digit_one=num%10;
	*digit_ten=num/10;

	return 0;	//����I��

}

//�\�������������l��7�Z�OLED�̊e���ɃZ�b�g����B
int set_segment(int value)
{
	int digit_one, digit_ten;

	num_to_2digit( value, &digit_one, &digit_ten);

	seven_segment_led[0]=seg7[digit_one];
	seven_segment_led[1]=seg7[digit_ten];
	seven_segment_led[2]=0xff;

	return 0;
}




void SysTick_Handler(void)
{

	int segment_digit;
	static int blank_count=0;
	
	//���荞�݊Ԋu�P�����A1��̃_�E���J�E���g�ɂP����
	if(TimeInt>0) TimeInt--;

	
	//�e���̕\���ؑւƁA�\���ؑւ̏u�Ԃɂ�������S���\��OFF�̃u�����N�����鏈��
	//�u�����N������̂́A���X�O����ꂷ�������B
	switch(blank_count){
	
		case 0:
			blank_count=1;
			segment_digit=0;	//1����
			break;
		case 1:
			blank_count=2;
			segment_digit=0;	//1����
			break;
		case 2:
			blank_count=3;
			segment_digit=0;	//1����
			break;
		case 3:
			blank_count=4;
			segment_digit=2;	//�\���u�����N
			break;
		case 4:
			blank_count=5;
			segment_digit=1;	//2����
			break;
		case 5:
			blank_count=6;
			segment_digit=1;	//2����
			break;
		case 6:
			blank_count=7;
			segment_digit=1;	//2����
			break;
		case 7:
			blank_count=0;
			segment_digit=2;	//�\���u�����N
			break;
	}
	
	
	GPIOB->ODR=digit[segment_digit];	//7seg�̕\������؂�ւ���
	GPIOA->ODR=seven_segment_led[segment_digit]<<4;	//7seg�ɐ�����\��


}







