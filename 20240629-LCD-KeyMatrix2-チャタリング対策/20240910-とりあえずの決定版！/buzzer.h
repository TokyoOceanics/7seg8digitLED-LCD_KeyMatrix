/*
 * buzzer.h
 *
 *  Created on: 2024/06/03
 *      Author: x-lib
 */

#ifndef BUZZER_H_
#define BUZZER_H_



//TIMによるブザー音階データ
#define C4 523 //ど
#define D4 587 //れ
#define E4 659 //み
#define F4 698 //ふぁ
#define G4 783 //そ
#define A4 880 //ら
#define B4 987 //し
#define C5 1046 //ど
#define D5 1174 //れ

#define LEN1	200
#define LEN		60

//音符データの構造体
typedef struct{
	int pitch;
	int duration;
} Note;



Note* melody_address;	//演奏させる曲の楽譜データの先頭アドレス




//ブザー出力関係
void play_tone(unsigned int frequency);
void stop_tone();
void play_music(Note data[]);
void keypad_clack_sound(void);
void inProgress_sound(void);


void Wait_Time(int);

#endif /* BUZZER_H_ */
