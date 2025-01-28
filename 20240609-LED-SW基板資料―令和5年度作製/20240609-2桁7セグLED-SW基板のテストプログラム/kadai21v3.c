#include "stm32f10x.h"
#include <stdlib.h>

#define C4 523 //��
#define D4 587 //��
#define E4 659 //��
#define F4 698 //�ӂ�
#define G4 783 //��
#define A4 880 //��
#define B4 987 //��
#define C5 1046 //��
#define D5 1174 //��

//�X�C�b�`�̒l���擾���邽�߂̃}�X�N
#define SW0 0x1
#define SW1 0x2

#define SW0_On 1
#define SW1_On 0

void setup_pwm(void);
void play_tone(unsigned int frequency);
void Wait_Time(int count);
void stop_tone(void);
unsigned int Get_SW(void);
void Seg7_Out_Data(unsigned int);


int main() {
    int delay = 2500000;


    unsigned int R4 ;//7�Z�O�_�ŗp
    unsigned int sw;//�ǂݍ��񂾃X�C�b�`�̒l���i�[����ϐ�
    int state = 0;//�X�C�b�`�̏�Ԃ�ۑ�


    setup_pwm();

    unsigned int melody[] =  {B4, G4, D4, G4, A4, D5
    							, D4, A4, B4, A4, D4, G4};
    int i =0;


    while (1){

    	sw = Get_SW();

    	if(sw & SW0){
    		state = SW0_On;
    	}else if(sw & SW1){
    		state = SW1_On;
    	}

    	if(state==0){
    		if(i==12)i=0;
    		if(i==5|i==11){
    			play_tone(melody[i]);
    			Wait_Time(delay*2);
    		}else{
    	   		play_tone(melody[i]);
    			Wait_Time(delay);
    		}

    		i++;


        // �����𐶐����A�����7�Z�OLED�ɏo��
    	R4 = ~0x0010;
    	R4 = R4 << (rand() % 8);
    	Seg7_Out_Data(R4);


    	}else{
    		stop_tone();
    	}


    }
}

unsigned int Get_SW(void){
	unsigned int sw;
	sw = GPIOC->IDR;
	sw =~sw;
	sw = sw & (SW0|SW1);

	return sw;
}


void Wait_Time(int count){
        for(int i = 0; i<count; i++){}
}

void setup_pwm() {
    // GPIOB��TIM3�̃N���b�N��L���ɂ���
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN|RCC_APB2ENR_IOPBEN|RCC_APB2ENR_IOPCEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // PBA��ݒ肷��i7�Z�O�p�j
    GPIOA->CRL =0x11114444;
    GPIOA->CRH =0x44441111;

    // PB0���ɐݒ肷��
    GPIOB->CRL = 0x4444444B;

    // PWM���[�h��I�����APWM�M�����s���ɏo�͂���
    TIM3->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2;
    TIM3->CCER |= TIM_CCER_CC3E;

    // �v���X�P�[����ݒ肷��
    TIM3->PSC = 720-1;

    // �^�C�}��L���ɂ���R���g���[�����W�X�^�B�@�^�C�}��ON�EOFF�Ȃǐݒ�
    TIM3->CR1 |= TIM_CR1_CEN;

    //���������[�h���W�X�^��ݒ肷��(�J�E���g����񐔂�ݒ肷��)
    //TIM3->ARR = 1000-1;
}

void play_tone(unsigned int frequency) {
    // ���������[�h���W�X�^�Ɣ�r���W�X�^�����g���ɉ����Đݒ肷��
    TIM3->ARR = (72000000 / frequency / 720) - 1;
    TIM3->EGR = TIM_EGR_UG;  // �ǉ�: ���W�X�^�̕ύX�������ɔ��f����
    TIM3->CCR3 = (TIM3->ARR + 1) / 2;  // 50% �f���[�e�B�T�C�N��
}

void stop_tone() {
    // PWM�M�����~����
    TIM3->CCR3 = 0;
}

void Seg7_Out_Data(unsigned int data){
	GPIOA -> ODR =data & 0x7f0;
	//7�Z�O�̐ڑ��|�[�g�ȊO�̃r�b�g���}�X�N����l
}


/*
PB0��TIM3�̃`���l��3

CCMR2
�^�C�}�[�̓��샂�[�h�𐧌䂷�郌�W�X�^�炵�� PWM���[�h��I�����邽�߂Ɏg�p

CCER
�o�͂�L���ɂ��邽�߂̃��W�X�^�炵�� PWM�M�����s���ɏo�͂��邽�߂ɕK�v

CCR3
PWM�̃f���[�e�B�T�C�N���𐧌䂷�郌�W�X�^�@�f���[�e�B�T�C�N���Ƃ́A1��PWM�T�C�N���̒��œd����HIGH�ł��鎞�Ԃ̊���

PWM�M���̎��g���ƃf���[�e�B�T�C�N���𐧌䂵�A���ʓI�Ƀu�U�[�̉��𐧌�

PWM�̎��g���́A�^�C�}�[�̃J�E���g�A�b�v���g���i�V�X�e���N���b�N���g�����v���X�P�[���[�Ŋ������l�j�ƁA�^�C�}�[�̃J�E���g�A�b�v���ꏄ����܂ł̒l�i���������[�h�l�j�ɂ���Č��܂�B

�V�X�e���N���b�N��72MHz�A�v���X�P�[���[��720�A���������[�h�l��1000�̏ꍇ�APWM�̎��g���́�

PWM���g�� = �V�X�e���N���b�N���g�� / �v���X�P�[���[ / ���������[�h�l
          = 72MHz / 720 / 1000
          = 100Hz
PWM�̎��g�������肷�邽�߂ɂ�
          ���������[�h�l = �V�X�e���N���b�N���g�� / �v���X�P�[���[ / PWM���g��
*/
