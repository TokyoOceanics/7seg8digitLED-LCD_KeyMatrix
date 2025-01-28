/*
 * 	00����FF�܂Ő��l��\������B
 * 	�u�����N���Ԃ�݂��āA�\�������₷������B
 *
 * 2023/07/25 ����@�R�����g�t�^
 */

/*
	�{�[�h��LED�ƃX�C�b�`�̐ڑ�
 	A552-SRD�i2����7�Z�OLED�\����j
 	SegA	PA4 CN7-32
 	SegB	PA5 CN10-11
 	segC	PA6 CN10-13
 	SegD	PA7 CN10-15
 	SegE	PA8 CN10-23
 	SegF	PA9 CN10-21
 	SegG	PA10 CN10-33
 	SEGDP	PA11 CN10-14

 	SelDig1	PB5 CN10-29
 	SelDig2	PB6 CN10-17

	�����{�^���X�C�b�`
 	SW1		PC0 CN7-38
 	SW2		PC1 CN7-36

 */



//#include <stddef.h>
#include "stm32f10x.h"

//�VsegLED�̕\���f�[�^�A���_��
int seg7[]={0xc0, 0xf9, 0xa4,0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90};

//7segLED�̊e�����ƂɁA�\���f�[�^��ێ�����ϐ�
//�@[0]�F1���ځA[1]:2���ځA[2]:���I��
int seven_segment_led[3];


//7segLED�̕\�����I���A���_��
//             76543210   76543210   76543210
int digit[]={0b11011111,0b10111111,0b11111111};
//           1���ڂ̕\��    2���ڂ̕\��   �S����

int TimeInt=0;	//Wait����Ԋu���w�肷�邽�߂̃O���[�o���ϐ��iSysTick�Ō��Z�j



//�v���g�^�C�v�錾
void my_init_system(void);
void Wait_Time(int);//SysTick_Handler���g���҂����Ԑ����֐�
int set_segment(int value);
int num_to_2digit(int num,int *digit_one, int *digit_ten);


int main(void)
{

	my_init_system();		//�|�[�g�̏�����



	while(1){
		for(int i=0; i<100; i++){
			set_segment(i);		//�J�E���^���li�ɑΉ�����7�Z�O�\���f�[�^���Z�b�g����
								//�l�́ASysTick_Handler�ɂ��ALED�ɕ\�������B

			Wait_Time(1000);	//1000msec�i��1�b�j�҂�
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
	GPIOA->CRH=0x44441111;	//PA8-PA11���o�͂Ƃ���B
//               76543210
	GPIOA->CRL=0x11114444;	//PA0-PA7���o�͂Ƃ���B

//               76543210
	GPIOB->CRL=0x41144444;	//PB5,PB6���o�͂Ƃ���B

//               76543210
	GPIOC->CRL=0x44444444;	//PC0-PC7�͑S���͂Ƃ���B
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

	seven_segment_led[0]=seg7[digit_one];	//1�̈ʂ�1����
	seven_segment_led[1]=seg7[digit_ten];	//10�̈ʂ�2����
	seven_segment_led[2]=0xff;				//�����̃f�[�^

	return 0;
}




void SysTick_Handler(void)
{

	int segment_digit;	//7�Z�O�k�d�c�̕\�����A�O�̂Ƃ�1���ځA�P�̂Ƃ�2���ځA�Q�̂Ƃ����I���B

	static int blank_count=0;	//7�Z�O�k�d�c�̕\����Ԃ𐧌䂷��ϐ�
	// blank_count=0,1,2�̂Ƃ��A1���ڂ�1�̈ʂ�\��
	//�@blank_count=3�̂Ƃ��A����
	// blank_count=4,5,6�̂Ƃ��A2���ڂ�10�̈ʂ�\��
	// blank_count=7�̂Ƃ��A�����B
	
	//���荞�݊Ԋu�P�����A1��̃_�E���J�E���g�ɂP����
	if(TimeInt>0) TimeInt--;

	
	//�e���̕\���ؑւƁA�\���ؑւ̏u�Ԃɂ�������S���\��OFF�̃u�����N�����鏈��
	//�Rmsec�\�������āA�Pmsec����������B���X�O����ꂷ�������B
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
	
	
	GPIOB->ODR=digit[segment_digit];	//�\�����ɍ��킹��7�Z�O�k�d�c�\���f�[�^���o�́B
	GPIOA->ODR=seven_segment_led[segment_digit]<<4;	//�\�����I��M�����o�́B7�Z�O�k�d�c�ɐ������\�������B


}







