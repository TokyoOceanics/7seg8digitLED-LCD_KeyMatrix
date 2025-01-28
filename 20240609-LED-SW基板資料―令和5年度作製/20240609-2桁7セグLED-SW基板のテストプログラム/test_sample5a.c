/*
 * 	Sw1,Sw2�̃e�X�g
 * 	Sw1��7seg��1���ڂ�S�_��
 * 	Sw2��7seg��2���ڂ�S�_��
 * 	SW1,Sw2���������ŁA7seg��S�_��
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


#define Delay_Time	10	//Wait���ԁ@1ms�P��

#define Sw1 0x01	//Sw1(PC0)�p�̃}�X�N�l
#define Sw2 0x02	//Sw1(PC1)�p�̃}�X�N�l



int set_segment(int value);
int num_to_2digit(int num,int *digit_one, int *digit_ten);

//
//�O���[�o���ϐ�
//
int TimeInt=0;	//delay�����iWait����Ԋu���w�肷�邽�߁j�̃O���[�o���ϐ��iSysTick�Ō��Z�j

int up=0;		//�X�C�b�`�̗����オ�茟�o�t���O�A0�ŏ�����
int sw_now=0;	//���݂̃X�C�b�`������ԁA0�ŏ�����


int main(void)
{

	my_init_system();

	int count=0;	//�J�E���^�ϐ�


	while(1){
		set_segment(count);	//�J�E���^�̒l���V�Z�O�����g�\����ɕ\��

		switch(GPIOC->IDR&(Sw1|Sw2)){
			case Sw1:
				GPIOA->ODR=0;
				GPIOB->ODR=digit[0];
				break;
			case Sw2:
				GPIOA->ODR=0;
				GPIOB->ODR=digit[1];
				break;
			case Sw1|Sw2:
				GPIOA->ODR=0;
				GPIOB->ODR=~digit[0]|~digit[1];
				break;


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
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);	// SysTick �^�C�}�����݂̊Ԋu�� 1ms �ɐݒ�


	//GPIO�@A,B,C��L����
	RCC->APB2ENR = RCC_APB2ENR_IOPAEN |	// GPIOA �̃N���b�N��L���ɂ���
				   RCC_APB2ENR_IOPBEN |	// GPIOB �̃N���b�N��L���ɂ���
				   RCC_APB2ENR_IOPCEN;	// GPIOC �̃N���b�N��L���ɂ���

	//�|�[�gA,B,C�̓��o�͐ݒ�

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

	
	int segment_digit;	//7�Z�O�\�����ʒu�ԍ��@0��1���ځA1��2���ځA3�ŏ���
	static int blank_count=0;//7�Z�O�\�����̏�������p�̏�ԕϐ��A�ڍׂ͉��Lswitch-case�Q��

	//SysTick_Handler�ɂ��X�C�b�`�̃`���^�����O�����p�̃t���O��
	static int swr2=0;	//�Q�T���v���P�ʑO�̃X�C�b�`��Ԃ��i�[����ϐ�
	static int swr1=0;	//�P�T���v���P�ʑO�̃X�C�b�`��Ԃ��i�[����ϐ�
	static int swr0=0;	//�����_�ŃT���v�������X�C�b�`��Ԃ��i�[����ϐ�

	static int sw_last=0;	//�O��m�肳�����X�C�b�`�������

	//
	//�X�C�b�`�̃`���^�����O�����̏���
	//

	swr2=swr1;	//2�T���v���O�̃X�C�b�`��ԂƂ��āA1�T���v���O�̃X�C�b�`��Ԃ�ޔ��i�[
	swr1=swr0;	//1�T���v���O�̃X�C�b�`��ԂƂ��āA�����_�̃X�C�b�`��Ԃ�ޔ��i�[

	swr0=GPIOC->IDR&(Sw1|Sw2);	//�����_�̃X�C�b�`��Ԃ��擾���A�K�v�������}�X�N���o

	if((swr2==swr1)&&(swr1==swr0)){//2�T���v���O�A1�T���v���O�A�����_�̃X�C�b�`��Ԃ��������Ȃ�A����������_�̃X�C�b�`��ԂƂ��Ċm��
		sw_now=swr0;
	}

	//�X�C�b�`�����̌��o�i����茟�o�j
	if((sw_now!=sw_last)){	//�����_�̃X�C�b�`��Ԋm��l�ƁA�O��̃X�C�b�`��Ԋm��l����v���Ȃ��ꍇ
		up=sw_now&~sw_last;	//����茟�o�t���O��ݒ�
		sw_last=sw_now;		//���݂̃X�C�b�`��Ԋm��l���X�V
	}


	//
	//�҂����Ԋ֐��@Wait_Time()�p�̕ϐ��̏���
	//

	//���荞�݊Ԋu�P�����A1��̃_�E���J�E���g�ɂP����
	if(TimeInt>0) TimeInt--;

	

	//
	//7�Z�OLED�\������
	//

	//�e���̕\���ؑւƁA�\���ؑւ̏u�Ԃɂ�������S���\��OFF�̃u�����N�����鏈��
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







