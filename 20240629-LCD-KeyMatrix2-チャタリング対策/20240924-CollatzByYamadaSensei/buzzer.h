/*
 * buzzer.h
 *
 *  Created on: 2024/06/03
 *      Author: x-lib
 */

#ifndef BUZZER_H_
#define BUZZER_H_



//TIM�ɂ��u�U�[���K�f�[�^
#define C4 523 //��
#define D4 587 //��
#define E4 659 //��
#define F4 698 //�ӂ�
#define G4 783 //��
#define A4 880 //��
#define B4 987 //��
#define C5 1046 //��
#define D5 1174 //��

#define LEN1	200
#define LEN		60

//�����f�[�^�̍\����
typedef struct{
	int pitch;
	int duration;
} Note;



Note* melody_address;	//���t������Ȃ̊y���f�[�^�̐擪�A�h���X




//�u�U�[�o�͊֌W
void play_tone(unsigned int frequency);
void stop_tone();
void play_music(Note data[]);
void keypad_clack_sound(void);
void inProgress_sound(void);


void Wait_Time(int);

#endif /* BUZZER_H_ */
