/*
 * buzzer.c
 *
 *  Created on: 2024/06/03
 *      Author: x-lib
 */

/*
 * PB0 Buzzer
 *
 *

 * PB0�̏������̃��� �܂��Ȃ��Ƃ��āB

  	//TIM3 �L�������āAPB0��TIM3�Ƃ���
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;	//TIM3�̗L����


  	//�|�[�gB�̓��o�͐ݒ�@GPIOB->CRL�ݒ�́A�|�[�gB�̓��o�͐ݒ�@pb0��TIM3�Ƃ���
	//PB0�@CNF=10�AMODE=11�́@�I���^�l�[�g�o�́E�v�b�V���v��
	//			 76543210
	GPIOB->CRL=0x???????B;

    // PWM���[�h��I�����APWM�M�����s���ɏo�͂���
    TIM3->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2;
    TIM3->CCER |= TIM_CCER_CC3E;

    // �v���X�P�[����ݒ肷��
    TIM3->PSC = 720-1;

    // �^�C�}��L���ɂ���R���g���[�����W�X�^�B�@�^�C�}��ON�EOFF�Ȃǐݒ�
    TIM3->CR1 |= TIM_CR1_CEN;
 *
 *
 *�g����
 	melody_address=melody[1];�@//�y�ȃf�[�^�̐擪�A�h���X
  	play_music(melody_address);//���t



 2024/06/05
 keypad_clac_sound�֐��AinProgress_sount�֐��ɂ����āA
 WaitTime�ɂ��f�B���C�iSysTick�g�p�j���~�߂��Ƃ���A�Ӑ}���铮��ƂȂ����B

 *
 */
#include <stm32f10x.h>
#include "buzzer.h"

//extern const Note melody[][20];

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




void play_music(Note data[]){
	int i=0;
	while(data[i].pitch!=0){
		play_tone(data[i].pitch);
		Wait_Time(data[i].duration);
		i++;
		//time_data=i;
		stop_tone();
	}

}


void keypad_clack_sound(void){

	//�L�[�{�[�h�Ō���

//	Note mldy[]={{C4,1000},{D4,10},{0,0}};
//	play_music(mldy);

	play_tone(C4);
	delay_us(10000);
	stop_tone();
	play_tone(C5);
	delay_us(10000);
	stop_tone();

}

void inProgress_sound(void){

	play_tone(D5);
	delay_us(10000);
	stop_tone();
	play_tone(C4);
	delay_us(100);
	stop_tone();

}


