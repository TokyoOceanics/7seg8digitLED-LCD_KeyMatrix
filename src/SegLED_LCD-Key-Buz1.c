/*
 * 7SegLED_LCD-Key-Buz1.c
 *
 *  Created on: 2024/10/08
 *      Author: x-lib
 *
 *
 *�@LCD���W���[����7SegLED�~8���\����ƃL�[�}�g���N�X����m�F�v���O����
 *
 *�@�͌��搶�̃{�[�h�A�H���d�q�̂S���R�L�[�}�g���N�X���g�p

 * 2024/05/06
 * keypad_getkey�ɂ����āAcol�̒l�����f����Ȃ��̂ŁA�������L�[�R�[�h�������Ȃ��B
 *
 * �z���̐ڐG�s�ǂɂ��ALCD�\���������B����̓��[�v�擪��LCD�����������邱�ƂőΉ��B
 *
 *2024/05/08
 *�X�C�b�`�}�g���N�X�̌���
 *�󋵕ς�炸�B�����āALCD�\���������������悤�ɂȂ�B
 *
 *2024/05/10
 *�Z�ŕs���́A�X���b�V�����h�s�������̈ꎞ���̂��΍�ł̃A�[�V���O�����i���a�搶�w�E�ɂ��j
 *�s���̎w���ԈႢ�A
 *
 *keypad_getkey�֐��ɂ�����xyz�M�����̃r�b�g�Z�b�g�E���Z�b�g�̊��Ⴂ(���Z�b�g��������Ă����j
 *
 *2024/05/12 �ˌ�����
 *�L�[�̑Ō����A������LCD�ɔ��f�����悤�ɂȂ�B�������A���܂����̓��삠��B
 *���I�����COL�l���Ӑ}�����l�ɂȂ��Ă��Ȃ����A�����L�[�ɑ΂���Ԃ�l�͂��܂������Ă���B
 *���ς�炸�A�����Ԉ���͂������̂́ANo29���}�C�R���ɂ����ẮALCD�̕\�����s����B
 *
 *���p�����1���@�����������̂ł͂Ȃ����ƂȂ��Ă����B
 *
 *�`���^�����O�h�~��
 *
 *
 *2024/05/13
 *����1���@�ɂ����āALCD����̔z����GND�Ƃ̃s�����������A�ڐG�s�ǂ𔭌��A
 *����ɑΉ������Ƃ���A�{�[�h�͓��삷��悤�ɂȂ����B
 *
 *2024/05/16
 *�悤�₭�A�`���^�����O�h�~���l������L�[���͂�����
 *�������A�����L�[���͂��󂯕t���Ȃ��s�
 *
 *2024/05/18 01:38
 *�悤�₭���삵���B
 *
 *2024/06/02 �L�[���͂𐔒l�Ƃ��鏈����ǉ��A���l���\�������悤�ɂȂ����B
 *
 *collatz�̏��������āA���삳����B�k�b�c��ɓ����Ԃ�\��������B�@06/02 17:40
 *
 *
 *2024/06/03
 *Buzzer�����ǉ��ɒ���
 *2024/06/04
 *���삵�n�߂�B�ϐ���buzzer.h�ɂĒ�`���Ă��āA2�d��`�ŃG���[���o�Ă����̂���������B
 *�L�[�p�b�h�Ō��������܂������܂������Ȃ��B
 *
 *2024/06/05
 *�L�[�p�b�h�Ō������o��悤�ɂȂ�B�Ȃ����Akeypad_clack_sound�֐����ɂ����āA
 *SysTick���g�����荞��Wait_Time���g���ƁA�\���I�󋵂ƂȂ�B
�@*��ނ𓾂��A�A�C�h�����[�v�Ńf�B���C��delay_us��]�p�����Ƃ���A�Ӑ}���铮��ƂȂ�B

 20240603-

 *2024/06/06
 *7Seg�����J�n

 2024/06/29 �ĊJ
 �L�[���͂��n�߂āA���̎��ɑŌ����鐔���L�[�Ō����
Queue_to_number_array(&queue, num_on_7segLED);
�ɓ˓�����ƁAHard Fault����B

2024/07/01
�L�[���͉ߒ��̏󋵂�7�Z�O�k�d�c��ɕ\�������邱�Ƃ��o����悤�ɂȂ�B
�������A�v�Z�ߒ��̕\����7�Z�O�k�d�c��ɕ\��������ƁA�k�b�c�\�������邱�Ƃ����o�B
�����ɁA�k�b�c�Ƃk�d�c�Ƃ͐��䂪�ł��Ȃ��̂��Bread modfy write�̋Z�͎g���Ă�����肾���B
chatGPT�̐�������R�[�h�����p���Ă݂����A���܂����̗l�q�B


2024/07/03
�\���̕s��́A������������ABuz�iPB0�j�ɂ���̂�������Ȃ��B

2024/07/11
���̔łł܂��͌Œ肵�A���ނƂ��Ē�


2024/09/06
get_number�̏�ԑJ�ڐ݌v�̌�����
�o�O���܂炸�B

2024/09/08
get_number�̌��������ĊJ�B����ǁA��O���͎��̃��J�o���[�ɕs�����B

2024/09/10
get_number�֐����ł́A�ē��͏����i�������j��������߂ē����P����

2024/10/03 T.Yamada
lcd�ւ̃f�[�^���M��(ODR��������+strobe)�Ɉȉ����s���B
	�E7Seg�\���X�V�������~�߂�Benable_segment_display<-DISABLE or ENABLE��
	�@���p����B
	�ELCD��7Seg��ODR���p�r�b�g�ɑ΂��鑀��𕜌����āALCD�̑��삪�Ȃ��������Ƃɂ���B���p4
	�@�r�b�g��LCD���̏����ł͓]���̂��߂̈�ߐ��̗��p�ł���̂ŁA���̃f�[�^��strobe���
	�@�K�v�Ƃ��Ȃ��B����ɑ΂��āA7Seg���͕\���̂��߂Ɍ��p4�r�b�g��ɑ��݂���K�v�����邽�߁A
	�@LCD�̏������ς񂾂�A���₩��7Seg�f�[�^�𕜌����Ēu���K�v������B

��L�̉��P�ŕω����Ȃ���΁A�ȉ����s���B
	�E�L�[�̓��͑҂����Ȃ����āA���C�����[�v��7Seg�\���X�V���s���悤�ɂ���B
	�E�L�[�̓��͂�1ms���ł͂Ȃ��A10ms���ɕς���B



*/

/*
 * 4x3�L�[�p�b�h�i�L�[�}�g���N�X�j�̔z��
 *
 * A	PA10	CN10-33
 * B	PA9		CN10-21
 * C	PA8		CN10-23
 * D	PA7		CN10-15
 *
 * Z	PA6		CN10-13
 * Y	PA5		CN10-11
 * X	PA4		CN7-32
 */


/*
 *�@LCD���W���[���̔z��
 *
 * 	D7	PB15	CN10-26	(7SegmentLED���W���[����Segment DP�ƌ��p�j
 * 	D6	PB14	CN10-28	(7SegmentLED���W���[����Segment G�ƌ��p�j
 * 	D5	PB13	CN10-30	(7SegmentLED���W���[����Segment F�ƌ��p�j
 * 	D4	PB12	CN10-16	(7SegmentLED���W���[����Segment E�ƌ��p�j
 *
 * 	RS	PB7		CN7-21
 * 	E	PB6		CN10-17
 *
 *
 */


/*
 * 7 SegmentLED���W���[���A�u�U�̔z��
 *
 * SegDP~SegA	PB15~PB8
 * Dgt7~Dgt0	PC7~PC0
 * Buzzer		PB0
 * UserButtonSW	PC13
 *
 *							//y LCD ���p
 *SegDP	PB15	CN10-26     // D7
 *SegG	PB14	CN10-28     // D6
 *SegF	PB13	CN10-30     // D5
 *SegE	PB12	CN10-16		// D4
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




#include <queue_tool.h>
#include <buzzer.h>
#include <stm32f10x.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//�֐��̃v���g�^�C�v�錾


void lcd_init(void);
void lcd_sendCommand(unsigned char cmd);
void lcd_sendData(unsigned char data);
void lcd_print(char * str);
void lcd_gotoxy(unsigned char x, unsigned char y);
void lcd_all_clear(void);
void lcd_clear_line(int line);

void delay_us(uint16_t t);



void my_init_system(void);
void Wait_Time(int time);	//SysTick_Handler���g���҂����ԏ���

void SelectDigit(int digit);
void Display_7Seg(int number);



uint8_t keypad_kbhit();
uint8_t keypad_getkey();
void read_key(void);
void wait_any_key_input(void);

int get_number(int *number);
//int disp_queue_on_7_segment_LED(QUEUE_T*);
void Queue_to_number_array(QUEUE_T* queue, int num[]);

int Mister_Collatz(int start_number);


void Debug_Blink_OnBoard_LED(int , int);


#define LCD_RS	7
#define LCD_EN	6

#define DELAY	1000


/****************************************************************/
#define DISP_7SEG	//7SegmentLED�Ɍv�Z�r���\������ꍇ�́A�R�����g�A�E�g����B
/****************************************************************/

#define UserButtonSW	(GPIOC->IDR)&(0x01<<13)

#define PUSHED	0xffff
#define ENABLE	0xff
#define DISABLE	0x0


//�O���[�o���ϐ�
int TimeInt=0;	//delay�����iWait����Ԋu���w�肷�邽�߁j�̃O���[�o���ϐ��iSysTick�Ō��Z�j

int key_pushed_flag=0; 			//�L�[�p�b�h�������o�t���O

uint8_t determined_key_code;	//�����_�ł̃X�C�b�`�̊m��l

int num_on_7segLED[8];	//7 �Z�O�����g�k�d�c�ɕ\�����鐔�l

int enable_segment_display=DISABLE;	//7�Z�O�����g�k�d�c�\������


//�y�Ȃ̉����f�[�^
const Note melody[][20]={
		//���y�搶
		{{D5,LEN1},{B4,LEN1},{A4,LEN1},{G4,LEN1},{F4,LEN1},{E4,LEN1},
				{F4,LEN1},{C5,LEN1},{C5,LEN1},
				{B4,LEN1},{A4,LEN1},{G4,LEN1},{F4,LEN1},{F4,LEN1},{E4,LEN1},{D4,LEN1},{C4,LEN1},{C4,LEN1},{0,0}},
		//�t�@�~���[�}�[�g
		{{B4,LEN1},{G4,LEN1},{D4,LEN1},{G4,LEN1},{A4,LEN1},{D5,LEN1*2},
				{D4,LEN1},{A4,LEN1},{B4,LEN1},{A4,LEN1},{D4,LEN1},{G4,LEN1*2},{0,0}},
		//���炫�琯
		{{C4,LEN1},{C4,LEN1},{G4,LEN1},{G4,LEN1},{A4,LEN1},{A4,LEN1},{G4,LEN1*2},
				{F4,LEN1},{F4,LEN1},{E4,LEN1},{E4,LEN1},{D4,LEN1},{D4,LEN1},{C4,LEN1*2},{0,0}},
		//
		{{C4,LEN1},{D4,LEN1},{E4,LEN1},{F4,LEN1},{G4,LEN1},{A4,LEN1},{B4,LEN1},{C5,LEN1},{D5,LEN1*2},{0,0}},
		//
		{{D5,10},{C4,10},{0,0}}

	};

#if 0
/*
 * https://nicerland.com/stm32f103/
 * ��9�͂̃v���O�����̓]�p
 */
int main()
{

	my_init_system();//�n�[�h�E�F�A�̏�����

	lcd_init();

	lcd_gotoxy(1,1);				//move cursor to 1,1
	lcd_print("The world is but");	//show "The world is but"
	lcd_gotoxy(1,2);				//move cursor to 1,2
	lcd_print("one country");		//show "one country"

	Debug_Blink_OnBoard_LED(3, 500);
	while(1);
}
#endif













#if 0
/*
 *  �k�b�c�\�����*���i���ɕ\������B
 *  �J�[�\���ʒu��1�����ÂE�ɓ������Ȃ���B
 *
 */

int main(void){


	my_init_system();
	lcd_init();

	while(1){
	for(int line=1; line<=2; line++)
		for(int col=1; col<=16; col++){
			lcd_gotoxy(col,line);
			lcd_sendCommand(0x0e);	//display on, cursor on
			lcd_sendData('*');
			Debug_Blink_OnBoard_LED(2,100);
			Wait_Time(300);
			lcd_sendCommand(0x01);
		}
	}
}

#endif



#if 0
/*
 *
 *�@LCD�ɕ������\��
 *�@LCD�ɕ������\��7�Z�OLED�Ɉꌅ����0-3��\�����āA��������O���猅�V�܂ŌJ��Ԃ�
 *
 */


int main()
{

	char str[17];

	my_init_system();//�n�[�h�E�F�A�̏�����

	loop:

	lcd_init();

	lcd_sendData(0x01);				//LCD clear
	lcd_gotoxy(1,1);				//move cursor to 1,1
	lcd_print("The world is but");	//show "The world is but"
	lcd_gotoxy(1,2);				//move corsor to 1,2
	lcd_print("one country");		//show "one country"


	while(1){
		for(int digit=0; digit<=7; digit++){
			SelectDigit(digit);
			for(int data=-1; data<4; data++){



				lcd_all_clear();
				sprintf(str,"[%d]",data);
				lcd_gotoxy(1,2);
				lcd_print(str);

				Display_7Seg(data);


				Wait_Time(500);

			}
		}
	}

	goto loop;
}

#endif



#if 0
/*
 *
 *�@LCD�ɕ������\��
 *�@LCD�ɕ������\��7�Z�OLED�Ɉꌅ����0-3��\�����āA��������O���猅�V�܂ŌJ��Ԃ�
 *
 */


int main()
{

	my_init_system();//�n�[�h�E�F�A�̏�����

	loop:

	lcd_init();

	lcd_sendData(0x01);				//LCD clear
	lcd_gotoxy(1,1);				//move cursor to 1,1
	lcd_print("The world is but");	//show "The world is but"
	lcd_gotoxy(1,2);				//move corsor to 1,2
	lcd_print("one country");		//show "one country"


	while(1){
		for(int digit=0; digit<=7; digit++){
			SelectDigit(digit);
			for(int data=0; data<4; data++){
				Display_7Seg(data);
				Wait_Time(100);

			}
		}
	}

	goto loop;
}

#endif







#if 0
/*
 * �L�[�{�[�h����̓��͂��k�b�c�ɕ\��
�@*�@https://nicerland.com/stm32f103/��9�͂̃v���O�������H���̃L�[�p�b�h�ɈڐA
 */

int main()
{

	my_init_system();//�n�[�h�E�F�A�̏�����

	lcd_init();
	lcd_gotoxy(1,1);	/* move cursor to 1,1 */
	lcd_print("The pressed key is: "); /* show the message */

	while(1)
	{

//		uint8_t data=keypad_getkey();

		if(keypad_kbhit() == 0){
			lcd_gotoxy(1,2);	/* move cursor to 1,2 */
			lcd_print("-");
			lcd_gotoxy(4,2);	/* move cursor to 4,2 */
			lcd_print("-");

		}else{
			lcd_gotoxy(1,2);	/* move cursor to 1,2 */
			lcd_sendData(keypad_getkey()); /* show the pressed key */

#ifdef	DEBUG
			lcd_gotoxy(4,2);	/* move cursor to 4,2 */

			lcd_sendData(COL+'0');
#endif

		}
	}
}

#endif




#if 0
/*
 *
 * �L�[���͂��󂯕t���āALCD�ɕ\������B
 * �`���^�����O�h�~�@�\�t���B
 * 2024/05/16 23:00�悤�₭�������B
 *
 *
 */


int main(void){


	int i=1;	//�Ō����������̕\�����ʒu�������΂񍶑��A�s���ɂ��Ă����B

	my_init_system();//�n�[�h�E�F�A�̏�����

	lcd_init();
	lcd_gotoxy(1,1);	/* �J�[�\����1�s�ڂ�1��ځA���Ȃ킿�s���Ɉړ����� */
	lcd_print("The pressed key is: "); /* ���b�Z�[�W��\��(������v�����v�g�j */

	while(1){
		while(key_pushed_flag!=PUSHED);	//�L�[���͂�����܂ő҂B
		key_pushed_flag=0;				//�L�[���͂��������̂ŁA�����t���O���N���A����B

		lcd_gotoxy(i++,2);				//2�s�ڂ�i��Ɉ󎚂��A���̕�����\������ʒu���ꕶ�����E�Ɉړ�����
		lcd_sendData(determined_key_code);//������\������B

		if(i>16) i=1;	//�s���܂ŕ\��������A�s���ɖ߂�

	}
}

#endif



#if 0
/*
 * Buzzer �e�X�g
 */
int main(void){

	my_init_system();

	melody_address=melody[0];	//�y�ȃf�[�^�̐擪�A�h���X
	play_music(melody_address);	//���t

  	while(1);

}
#endif




#if 0

int main(void){
	while(1)
		for(int i=0; i<100; i++);
	while(1);
}

#endif


#if 1
/*
 *
 * �L�[���͂��󂯕t���āACollatz�����LCD�ɕ\������B
 *
 *
 */



int main(void){

	int number;						//�L�[�p�b�h�����荞�񂾐��l
	int digit;						//��荞�񂾐��l�̌���
	char number_string[MAX_NUM];	//LCD�ɕ\�����邽�߂̐��l�̐�����
	int collatz_count;
	char message[16+1];

	my_init_system();//�n�[�h�E�F�A�̏�����

	lcd_init();

	while(1){


		lcd_all_clear();			/* �v�����v�g�\�� */
		//lcd_init();
		lcd_gotoxy(1,1);
		lcd_print("Collatz Problem");
		lcd_gotoxy(1,2);
		lcd_print("++ж��� ���޲++");

		melody_address=melody[1];	//�t�@�~���[�}�[�g���X�������t
		play_music(melody_address);	//

		do{

			digit=get_number(&number);	//�L�[�p�b�h����̓��͑҂��B�����L�[�Ō���A#�œ��͏I���B

			sprintf(number_string,"%d", number);//��荞�񂾐��l�𐔎���Ɋi�[�B

			lcd_clear_line(1);		//Collatz����̌v�Z�J�n�̃v�����v�g
			lcd_gotoxy(1,1);
			lcd_print("Number is ");
			lcd_print(number_string);

		}while(digit==0);


		lcd_clear_line(2);
		lcd_gotoxy(1,2);
		lcd_print("Any key to Start");
		wait_any_key_input();


		collatz_count=Mister_Collatz(number);	//Collatz����̌v�Z


		sprintf(message,"[%d] mks %d cnts",number,collatz_count);//Collatz����̌v�Z���ʕ\��
		lcd_clear_line(1);
		lcd_gotoxy(1,1);
		lcd_print(message);

		lcd_clear_line(2);
		lcd_gotoxy(1,2);
		lcd_print("Hit any key.");

		melody_address=melody[0];	//�������t
		play_music(melody_address);	//

		wait_any_key_input();


	}



}




//void keypad_clack_sound(void){
//
//	//�L�[�{�[�h�Ō���
//
////	Note mldy[]={{C4,10},{D4,10},{0,0}};
////	play_music(mldy);
//
////	int i=0;
////	for(i=0; i<10; i++);
//
//	play_tone(C4);
//	delay_us(1000);
//	play_tone(C5);
//	delay_us(1000);
//
//}



#endif












/*
 * LCD�̃f�[�^�������݃p���X�i�X�g���[�u�M���j�����@lcd_putValue�֐�����Ăяo�����
 *
 * LCD���W���[����D7�`D4��Ƀf�[�^�m�肵����A�X�g���[�u�M��EN�̗�����p���X���o���΁A
 * ���̃f�[�^���������܂��B
 */


void lcd_strobe(void)
{
	//�X�g���[�u�M���iE�̗�����j�ŁALCD�̃��W�X�^�Ƀf�[�^��]��
	GPIOB->ODR |= (1<<LCD_EN);	// EN=1 for H-to-L pulse
	delay_us(DELAY);
	GPIOB->ODR &= ~(1<<LCD_EN);	// EN=0 for H-toL pulse
	delay_us(DELAY);


}

/*
 * LCD���W���[���փf�[�^���������ފ֐�
 *
 * �����Ŏw�肳���value�̏��4�r�b�g���������݁A���ɉ���4�r�b�g���������ށB
 * ���̊֐��́ALCD���W���[���\������֐��̈�ԒႢ���C���[�Ƃ��ċ@�\����B
 */

void lcd_putValue(unsigned char value)
{
	//y �k�d�c�̕\�����~�߂�
	enable_segment_display = DISABLE;
	//y 7Seg�p���8�r�b�g�̌���ޔ� 2024/10/06
	int16_t save = GPIOB->ODR | 0xff00;

	/*�g�����ꂽ�A�r�b�g�Z�b�g�A���Z�b�g�̂������g���B*/
	//y GPIOB->ODR&=~0xf000;	//���4�r�b�g�̏�������
	GPIOB->BRR = 0xf000;		//���4�r�b�g�̃[���N���A
	//y GPIOB->ODR|=(value<<8)&0xf000;
	GPIOB->BSRR = (value << 8) & 0xf000;	//���4�r�b�g�ւ̃Z�b�g
	delay_us(DELAY);

	lcd_strobe();

	//y GPIOB->ODR&=~0xf000;	//����4�r�b�g�̏�������
	GPIOB->BRR = 0xf000;		//���4�r�b�g�̃[���N���A
	//y GPIOB->ODR|=(value<<12)&0xf000;
	GPIOB->BSRR = (value << 12) & 0xf000;	//���4�r�b�g�ւ̃Z�b�g
	delay_us(DELAY); //y �ǉ�

	lcd_strobe();

	//y 7Seg�p���8�r�b�g�̌��󕜋A 2024/10/06
	GPIOB->BRR = 0xff00;		//���8�r�b�g�̃[���N���A
	GPIOB->BSRR = save;			//���8�r�b�g�̕���
	//y �k�d�c�̕\�����ĊJ
	enable_segment_display = ENABLE;
}


/*
 * LCD���W���[���ɑ΂��āA����R�}���h�𑗂�֐�
*/
void lcd_sendCommand(unsigned char cmd)
{
	//y GPIOB->ODR &= ~(1<<LCD_RS);	//RS =0 for command
	GPIOB->BRR = (1<<LCD_RS);	//RS =0 for command
	delay_us(DELAY);

	lcd_putValue(cmd);

}

/*
 * LCD���W���[���ɑ΂��āA�f�[�^(��ɕ����j�𑗂�֐�
*/

void lcd_sendData(unsigned char data)
{
	//y GPIOB->ODR |=(1<<LCD_RS);	//RS=1 for data
	GPIOB->BSRR = (1<<LCD_RS);	//RS=1 for data
	delay_us(DELAY);

	lcd_putValue(data);


}

/*
 * LCD���W���[���ɕ������\������֐�
 */
void lcd_print(char* str)
{
	unsigned char i=0;
	while(str[i]!=0){
		lcd_sendData(str[i]);
		i++;
	}
}


/*
 * LCD���W���[��������������֐�
 */
void lcd_init()
{
	GPIOB->ODR &=~(1<<LCD_EN);	//LCD_EN=0
	delay_us(DELAY);



	//���ЃI���W�i���̃V�[�P���X
	//0x30 -> 0x30 -> 0x30 -> 0x20
	lcd_sendCommand(0x33);	//8bit mode 7654
							//          0011
	lcd_sendCommand(0x32);	//functionSet 4bit

	lcd_sendCommand(0x28);	//functionSet 4bit, 2line, 5x7font�@�璷�Ȃ���A4�r�b�g���[�h������x���{

	lcd_sendCommand(0x0e);	//display on, cursor on
	lcd_sendCommand(0x01);	//clear LCD
	delay_us(DELAY);
	lcd_sendCommand(0x06);	//shift cursor right


}



/*
 * �҂����Ԋ֐��i�A�C�h�����[�v�ɂ��j
 */
void delay_us(uint16_t t)
{
	volatile unsigned long l=0;
	for(uint16_t i=0; i<t; i++)
		for(l=0; l<6; l++);
}


/*
 * �J�[�\�����w��ʒu�Ɉړ�����֐�
 *�@x�͌��ʒu�Ay�͍s
 */
void lcd_gotoxy(unsigned char x, unsigned char y)
{
	const unsigned char firstCharAdr[]={0x80,0xc0, 0x94,0xd4};
	lcd_sendCommand(firstCharAdr[y-1]+x-1);
	delay_us(100);
}

/*
 * LCD�S�s����
 */
void lcd_all_clear(void)
{
	lcd_sendCommand(0x01);
}


/*
 * LCD�̎w��s�̕\���������@2024/06/02
 */
void lcd_clear_line(int line){
	lcd_gotoxy(1,line);
	lcd_print("                ");
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
	/*SysTick �^�C�}�����݂̊Ԋu�� 10ms �ɐݒ� */
	// ���荞�݊Ԋu��SysTick_Config()�̈�����RCC_Clocks.HCLK_Frequency / 1000�@�Ƃ��āA1msec
	//������100�ɂ��āAWaitTime(1000)��10�b�҂��ɂȂ�j
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);	//

	//GPIO�@A,B,C��L����
	RCC->APB2ENR|=RCC_APB2ENR_IOPAEN|RCC_APB2ENR_IOPBEN|RCC_APB2ENR_IOPCEN;

	//TIM3 �L�������āAPB0��TIM3�Ƃ���
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;	//TIM3�̗L����

	//			 76543210
	GPIOA->CRL=0x41114444;
	//			 FEDCBA98
	GPIOA->CRH=0x44444444;


	//�|�[�gB�̓��o�͐ݒ�@GPIOB->CRL�ݒ�́A�|�[�gB�̓��o�͐ݒ�@pb0��TIM3�Ƃ���
	//PB0�@CNF=10�AMODE=11�́@�I���^�l�[�g�o�́E�v�b�V���v��
	//			 76543210
	GPIOB->CRL=0x1144444B;
	//			 FEDCBA98
	GPIOB->CRH=0x11111111;

	//�|�[�gC
	//			 76543210
	GPIOC->CRL=0x11111111;
	//			 FEDCBA98
	//GPIOC->CRH=0x44444444;



    // PWM���[�h��I�����APWM�M�����s���ɏo�͂���
    TIM3->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2;
    TIM3->CCER |= TIM_CCER_CC3E;

    // �v���X�P�[����ݒ肷��
    TIM3->PSC = 720-1;

    // �^�C�}��L���ɂ���R���g���[�����W�X�^�B�@�^�C�}��ON�EOFF�Ȃǐݒ�
    TIM3->CR1 |= TIM_CR1_CEN;

}




void SelectDigit(int digit)
{

	if( (0<=digit) && (digit<=7) )
		GPIOC->ODR=(0x01<<digit);
	else
		GPIOC->ODR=0x00;

}




/*
 * �����搶�Z�؃R�[�h�]�p
 *
 *�@�k�b�c��EN(PB6),RS(PB7)�ɉe����^���Ȃ������̖͍�
 */
#if 0
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

		GPIOB->ODR &= ~(Seg7led_Table[number] /*& 0x7f00*/);	// 7 �Z�O�����g LED �ɕ\������
		GPIOB->ODR |= (Seg7led_Table[number] /*& 0x7f00*/);
	} else {											// 0 �` 9 �͈̔͊O�̒l���w�肳�ꂽ�ꍇ��
		GPIOB->ODR |= 0x00FF;
	}
}
#endif






/*
 * �����搶�Z�؃R�[�h�]�p
 *
 *�@�������A�k�b�c��EN(PB6),RS(PB7)���㏑�����Ă��܂����߂��A�k�b�c�\���������B
 */
#if 0
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
		GPIOB->ODR = Seg7led_Table[number] & 0x7f00;	// 7 �Z�O�����g LED �ɕ\������
	} else {											// 0 �` 9 �͈̔͊O�̒l���w�肳�ꂽ�ꍇ��
		GPIOB->ODR = 0x0000;							// �V�Z�O�����g LED ��S��������
	}
}
#endif


#if 0
/*
 * 2024/07/01
 * ChatGPT�̐����ɂ���
 * �����ɂ���R�[�h��ChatGPT�ɑ���A����8�r�b�g�ɉe����^���Ȃ��悤�ɂ������|�����肢������A
 * �������ꂽ�R�[�h�B
 *
 * LED�ɓr���o�߂͕\������邪�A�Ō�ɐ��������B�����āALCD�\����j��B
 */

void Display_7Seg(int number) {
    /*
    **      < 7 �Z�O�����g LED �̃|�[�g���蓖�� >
    **
    **             a        a - PB8
    **           ----        b - PB9
    **        f / g / b        c - PB10
    **          ----        d - PB11
    **       e /   / c        e - PB12
    **         ----            f - PB13
    **          d            g - PB14
    */

    /* 7 �Z�O�����g LED �\���p�^�[���p�̃f�[�^�z��i���_���Ƃ��Ē�`�j */
    const static int Seg7led_Table[] = { 0x3f00,    // 0
                                         0x0600,    // 1
                                         0x5b00,    // 2
                                         0x4f00,    // 3
                                         0x6600,    // 4
                                         0x6d00,    // 5
                                         0x7d00,    // 6
                                         0x2700,    // 7
                                         0x7f00,    // 8
                                         0x6f00     // 9
    };

    int temp_gpio;

    if ((number >= 0) && (number <= 9)) {                // ���l�� 0 �` 9 �͈̔͂Ȃ�w�肳�ꂽ�����̃p�^�[����
        temp_gpio = Seg7led_Table[number] & 0x7f00;

        // ����8�r�b�g��ی삵���8�r�b�g�����Z�b�g
        //y GPIOB->ODR &= 0x00FF;
        // ����8�r�b�g��ی삵���8�r�b�g��ݒ�
        //y GPIOB->ODR |= temp_gpio;
    	GPIOB->BSRR = temp_gpio;
    } else {                        // 0 �` 9 �͈̔͊O�̒l���w�肳�ꂽ�ꍇ�͂V�Z�O�����g LED ��S��������
        temp_gpio = 0xFF00;
        //y GPIOB->ODR &= ~temp_gpio;
        GPIOB->BRR = temp_gpio;
    }
}
#endif

#if 1
/*
 * �ŏ��̃R�[�h
 *
 *
 */
void Display_7Seg(int number) {
    /*
    **      < 7 �Z�O�����g LED �̃|�[�g���蓖�� >
    **
    **             a        a - PB8
    **           ----        b - PB9
    **        f / g / b        c - PB10
    **          ----        d - PB11
    **       e /   / c        e - PB12
    **         ----            f - PB13
    **          d            g - PB14
    */

    /* 7 �Z�O�����g LED �\���p�^�[���p�̃f�[�^�z��i���_���Ƃ��Ē�`�j */
    const static int Seg7led_Table[] = { 0x3f00,    // 0
                                         0x0600,    // 1
                                         0x5b00,    // 2
                                         0x4f00,    // 3
                                         0x6600,    // 4
                                         0x6d00,    // 5
                                         0x7d00,    // 6
                                         0x2700,    // 7
                                         0x7f00,    // 8
                                         0x6f00     // 9
    };

    int temp_gpio;

    if ((number >= 0) && (number <= 9)) {                // ���l�� 0 �` 9 �͈̔͂Ȃ�w�肳�ꂽ�����̃p�^�[����
        temp_gpio = Seg7led_Table[number] & 0x7f00;

        GPIOB->ODR = (GPIOB->ODR & 0x00FF) | temp_gpio;  // ����8�r�b�g��ی삵���8�r�b�g��ݒ�
    } else {                        // 0 �` 9 �͈̔͊O�̒l���w�肳�ꂽ�ꍇ�͂V�Z�O�����g LED ��S��������
        temp_gpio = 0xFF00;
        GPIOB->ODR &= ~temp_gpio;
    }
}
#endif



#if 0
/*
 * �k�b�c�̂q�r�Ƃd�m�ɉe����^���Ȃ��l�ɂ�������̃R�[�h�B�������A�k�d�c�}�g���N�X�ւ̕\���͂��܂������Ȃ��Ȃ����B
 *
 *�@LED��̕\���͕s��Ȃ���ALCD�ɂ͉e�����Ȃ��B
 */
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

	const static int Seg7led_Table[] = { 0x3f00,	// 0
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

	int temp_gpio;

	if ((number >= 0) && (number <= 9)) {				// ���l�� 0 �` 9 �͈̔͂Ȃ�w�肳�ꂽ�����̃p�^�[����
		temp_gpio= Seg7led_Table[number] & 0x7f00;

		GPIOB->ODR |= temp_gpio;	// 7 �Z�O�����g LED �ɕ\������
		GPIOB->ODR &= ~temp_gpio;
	} else {						// 0 �` 9 �͈̔͊O�̒l���w�肳�ꂽ�ꍇ�͂V�Z�O�����g LED ��S��������
		temp_gpio=0xFF00;
		GPIOB->ODR &= ~temp_gpio;
	}
}
#endif






/*
 * https://nicerland.com/stm32f103/
 * �̃R�[�h��]�p
 */
/* The function checks if a key is pressed or not */
/* Returns: 0: Not pressed, 1: Pressed */
uint8_t keypad_kbhit()
{
				   //FEDCBA9876543210
	GPIOA->ODR &= ~0b0000000001110000;		//A4,A5,A6��Low�ɂ���
	delay_us(1);
	if(((GPIOA->IDR>>7)&0xF) == 0xF) /* �����L�[��������Ă��Ȃ��ꍇ��0��Ԃ�*/
		return 0;
	else
		return 1;					/*����������Ă���΁A1��Ԃ�*/
}


/*
 * https://nicerland.com/stm32f103/
 * �̃R�[�h��]�p
 */
/* The function returns the pressed key. */
/* It returns 0 if no key is pressed. */

uint8_t keypad_getkey()
{
						// ColX ColY ColZ
	uint8_t keypadLookup[]={'1','2','3',	//RowD
							'4','5','6',	//RowC
							'7','8','9',	//RowB
							'*','0','#'};	//RowA
//	uint8_t character_code;

	const uint32_t colSelect[]={
		0x00100060, /* ColX ZYX=001 */
		0x00200050, /* ColY ZYX=010 */
		0x00400030};/* ColZ ZYX=100 */

	if(keypad_kbhit() == 0)	/* if no key is pressed */
		return 0;

	for(int col = 0; col <= 2; col++) /* col 0 to 2 */
	{
		GPIOA->BSRR=  colSelect[col]; /*�@ZYX�Z���N�g���Ă���񂾂���L�ɂ��� */
		delay_us(10);	/* wait for the columns to beupdated */
		uint8_t row = (GPIOA->IDR>>7)&0xF;

		switch(row)
		{
			case 0x0E:  /* rowD is low */
				return keypadLookup[0+col];

			case 0x0D: /* rowC is low */
				return keypadLookup[3+col];

			case 0x0B: /* rowB is low */
				return keypadLookup[6+col];

			case 0x07: /* rowA is low */
				return keypadLookup[9+col];

		}
	}

	return 0; //�Ԃ�l��0�̏ꍇ�́A�����{�^���̉��������B���̍s�̓R���p�C�����̌x���}���p�ɋL�q�B
}


/*
 * �R���S�L�[�p�b�h�̃L�[�����󋵂̎�荞�݊֐�
 *
 * �`���^�����O�h�~�@�\�t���B SysTick_Handler����Ăяo���Ďg�p����B
 *
 *�@��ʊ֐��ɂāA�L�[�o�b�h�����t���O�A�����_�ŉ������ꂽ�L�[�R�[�h�̊m��l�𒲂ׁA�L�[���͂̔��f������B
 *
 * �L�[�p�b�h�����t���Okey_pushed_flag
 * 0�̎��͉��������APUSHED�Ȃ牟������B
 *
 * �����_�ŉ������ꂽ�L�[�R�[�h�̊m��ldetermined_key_code
 * ������Ă���L�[�̃L�[�\�ʂɈ�����ꂽ�����R�[�h�������́A���������Ȃ�0�B
 *
 */
void read_key(void){
	static uint8_t last_determined_key_code;	//�O��܂ł̃X�C�b�`�̒l�̊m��l

	static uint8_t current_key_code =0;	//����ǂݍ��񂾃X�C�b�`�̒l
	static uint8_t previous1_key_code=0;//�O��ǂݍ��񂾃X�C�b�`�̒l
	static uint8_t previous2_key_code=0;//�O�X��ǂݍ��񂾃X�C�b�`�̒l



	previous2_key_code=previous1_key_code;	//����3�̃X�C�b�`�̒l���A�ʓ˂��ōX�V
	previous1_key_code=current_key_code;
	current_key_code=keypad_getkey();

	//����3�̃X�C�b�`�̒l�����ׂē�������΁A������m��l�Ƃ��ĕێ�����B
	if( (current_key_code==previous1_key_code) && (previous1_key_code==previous2_key_code)){
		determined_key_code=current_key_code;
	}

#if 0
//�ȉ��s��̗��j���L�^

	//�ȉ��̃��W�b�N�A�X�C�b�`�̉��������o�����Ƃ��́A�O��̊m��l�̈����A
	//�����t���O�̈����͋�J�����B2024/05/16 23:00
	//���܂̂Ƃ���A����L�[���͂��󂯓�����Ȃ��󋵁B

//	//���O�܂ł̊m��l�ƒ��߂̊m��l���������Ȃ���΁A
//	//�����_�ł̊m��l���O�ȊO�Ȃ�΁A�Ӗ��̂���L�[�̉��������������̂Ƃ��āA
//	//�����t���O�ƁA�O��̊m��l�Ƃ��X�V����B
//	if((determined_key_code!=last_determined_key_code)){
//		if((last_determined_key_code==0)&&(determined_key_code!=0)){
//			key_pushed_flag=PUSHED;
//			last_determined_key_code=determined_key_code;
//		}
//	}

//	if(determined_key_code != 0){
//		key_pushed_flag=PUSHED;
//		last_determined_key_code=determined_key_code;
//	}
#endif

	//�̂���������āA�悤�₭�ł����B2024/05/18 01:38
	if((last_determined_key_code==0)&&(determined_key_code!=0)){
		key_pushed_flag=PUSHED;
		keypad_clack_sound();
	}

	last_determined_key_code=determined_key_code;

	return;
}



void wait_any_key_input(void)
{
	while(key_pushed_flag!=PUSHED);	//�L�[���͂�����܂ő҂B
	key_pushed_flag=0;				//�L�[���͂��������̂ŁA�����t���O���N���A����B
	keypad_clack_sound();
}


void begin_keyin_prompt(void)
{
	lcd_all_clear();
	lcd_gotoxy(1,1);
	lcd_print("Hit * to initialize.");
}

void numbers_key_prompt(void)
{
	//2024�N7��5�����@����������
	lcd_init();
	//lcd_clear_line(1);
	lcd_gotoxy(1,1);
	lcd_print("Num(s), then #.");
}



/*
 * �L�[�p�b�h���琔�l����荞��
 *
 */
#define INITIAL		0
#define INPROGRESS	20
#define	POSTPROCESS	30
#define ERROR		40
#define	COMPLETE	50



#if 1
int get_number(int *number)
{
	int digit;						//���͂������l�̌���
	char number_string[MAX_NUM];	//���͂������l��LED�}�g���N�X�ɕ\�����邽�߂̕ϐ�

	QUEUE_T queue;					//�����L�[�̑Ō����ɕۑ�����L���[

	int state=INITIAL;
	begin_keyin_prompt();

	while(state!=COMPLETE){

		while(key_pushed_flag!=PUSHED);	//�L�[���͂�����܂ő҂B
		key_pushed_flag=0;	//�L�[���͂��������̂ŁA�����t���O���N���A����B

		keypad_clack_sound();
//		melody_address=melody[4];	//���������Ë��I�Ō���
//		play_music(melody_address);	//

		switch(state){

		case INITIAL:


			if(determined_key_code=='*'){
				initQueue(&queue);
				digit=0;
				numbers_key_prompt();

#ifdef DISP_7SEG
			enable_segment_display=ENABLE;	//7�Z�O�k�d�c�ɓ��͒l��\������
			memset(num_on_7segLED,0xff,sizeof(num_on_7segLED));
#endif



				state=INPROGRESS;
			}
			break;

		case INPROGRESS:

			if(determined_key_code=='#'){//#�L�[�����ŁA�L�[���͏I��
					state=COMPLETE;
			}else if(determined_key_code!='*'){//*�L�[�ȊO�i�����L�[�j�������̏���

				enqueue(&queue, determined_key_code);	//�Ō������L�[�R�[�h���L���[�Ɋi�[
				digit=QueueToString(&queue, number_string);//�Ō������������擾

				Queue_to_number_array(&queue, num_on_7segLED);//�L���[�̓��e��7�Z�O�k�d�c�\���p�z��ɃR�s�[

				lcd_clear_line(2);	lcd_gotoxy(1,2);	//LCD�ɒl��\��
				lcd_print(number_string);



			}
			break;
		}
	}

#ifdef DISP_7SEG
	enable_segment_display=DISABLE;	//7�Z�O�k�d�c�̕\�����~�߂�B
	SelectDigit(-1);	//7�Z�O�k�d�c�𖾎��I�ɏ�������
#endif

	sscanf(number_string,"%d",number);//������𐔒l�ɕϊ��Aget_number�̈����Ƃ��ČĂяo�����֓n���B

	return digit;


}

#endif

#if 0
int get_number(int *number){

	int digit;
	char number_string[MAX_NUM];

	QUEUE_T queue;

	int state=INITIAL;

	while(state!=COMPLETE){

		while(key_pushed_flag!=PUSHED);	//�L�[���͂�����܂ő҂B
		key_pushed_flag=0;	//�L�[���͂��������̂ŁA�����t���O���N���A����B

		keypad_clack_sound();
//		melody_address=melody[4];	//���������Ë��I�Ō���
//		play_music(melody_address);	//

		switch(determined_key_code){
			case '*':					//�L���[�̏�����
				initQueue(&queue);
				numbers_key_prompt();
				state=INPROGRESS;
				break;
			case '#':
				if(state!=INPROGRESS){
					break;
				}else{
					//printf("�L���[�̒��g��\��\n");
					//printQueue(&queue);
					state=COMPLETE;
					enable_segment_display=DISABLE;	//7�Z�O�k�d�c�̕\�����~�߂�B
					SelectDigit(-1);	//7�Z�O�k�d�c�𖾎��I�ɏ�������
					break;
				}

			default://�����L�[�̏ꍇ
				if(state!=INPROGRESS){
					//printf("Que is not ready. Hit # to initialize the que.\n");
					break;
				}else{
					//printf("[num=%d]\n",num);
					enable_segment_display=ENABLE;	//7�Z�O�k�d�c�ɓ��͒l��\������

					enqueue(&queue, determined_key_code);
					Queue_to_number_array(&queue, num_on_7segLED);
					break;
				}
		}
	}

	digit=QueueToString(&queue, number_string);
	sscanf(number_string,"%d",number);
	return digit;

}
#endif


/*
 * chatGPT�ɂ�萶�������֐�
 * �lnumber��^����ƁA���̒l��z��num_digit�Ɉꌅ���i�[����B
 * 8���ɖ����Ȃ��ꍇ�́A�[���l�߂������ɁA7�Z�O�����g�k�d�c�ɕ\�������Ȃ�99�ŋl�߂�B
 *
 */
//void intToDigits(int number, int num_digit[8]) {
//    for (int i = 0; i < 8; i++) {
//        if (number == 0) {
//            num_digit[i] = 99; // number��0�ɂȂ�����c���99�Ŗ��߂�
//        } else {
//            num_digit[i] = number % 10; // 1�̈ʂ̐������擾
//            number /= 10; // ���̈ʂɈړ�
//        }
//    }
//}

void intToDigits(int number, int num_digit[8]) {
    for (int i = 0; i < 8; i++) {
        if (number == 0) {
            num_digit[i] = 99; // number��0�ɂȂ�����c���99�Ŗ��߂�(0-9�ȊO�Ƃ����Ӗ��j
        } else {
            num_digit[i] = (number % 10); // 1�̈ʂ̐������擾
            number /= 10; // ���̈ʂɈړ�
        }
    }
}




void Queue_to_number_array(QUEUE_T* queue, int num[])
{
	int digit;
	int number;
	char number_string[MAX_NUM];


	digit=QueueToString(queue,number_string);
	number=atoi(number_string);

	intToDigits(number, num);

}


/*
 *



 *
*/






int Mister_Collatz(int start_number)
{
	int count=0;
	int val=start_number;
	char number_string[MAX_NUM];	//LCD�ɕ\�����邽�߂̐��l�̐�����
	char collatz_result_string[16+1];

	sprintf(number_string,"%d", start_number);

	lcd_clear_line(1);
	lcd_gotoxy(1,1);
	lcd_print("StartFrom ");
	lcd_print(number_string);

#ifdef DISP_7SEG
	enable_segment_display=ENABLE;	//7�Z�O�k�d�c�Ɍv�Z�ߒ���\������
#endif

	while(val!=1){
		if(val%2==0)
			val=val/2;
		else
			val=3*val+1;
		count++;

		//�v�Z�ߒ��̂k�b�c��̕\��
		sprintf(collatz_result_string,"[%d]-[%d]",count, val);
		//2024�N7��5���@����������
		lcd_all_clear();

		lcd_clear_line(2);
		lcd_gotoxy(1,2);
		lcd_print(collatz_result_string);

		//�v�Z�ߒ���7�Z�O�k�d�c��̕\��
		intToDigits(val, num_on_7segLED);

		//�v�Z�ߒ��̃T�E���h���
		inProgress_sound();

		Wait_Time(300);
	}

#ifdef DISP_7SEG
	enable_segment_display=DISABLE;	//7�Z�O�k�d�c�Ɍv�Z�ߒ��̕\�����~�߂�
#endif

	return count;
}




void Disp_number_on_7_segment_LED(void){


	static int digit_index=0;		//7�Z�O�ւ̕\�����A1�̈ʂ���\��

	if(digit_index==8){
		digit_index=0;
		SelectDigit(-1);
	}else{
		SelectDigit(digit_index);
		Display_7Seg(num_on_7segLED[digit_index]);
		digit_index++;	//���̌��̕\������
	}
	return ;

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

	//�R���S�L�[�p�b�h�̃L�[�����󋵂̎擾
	read_key();


	//
	//�҂����Ԋ֐��@Wait_Time()�p�̕ϐ��̏���
	//
	if(TimeInt>0) TimeInt--;

	if(enable_segment_display==ENABLE){
		Disp_number_on_7_segment_LED();
	}

}

#if 1

/*
 * �f�o�b�O�p F103�{�[�h�̃I���{�[�hLED��_��������B
 *
 */
void Debug_Blink_OnBoard_LED(int count, int delay)
{
	for(int i=0; i<count; i++){
		GPIOA->ODR^=(0x1<<5);
		Wait_Time(delay);
	}
}

#endif

