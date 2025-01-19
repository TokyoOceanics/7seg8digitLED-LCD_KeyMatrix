/*
 * 20240508-7SegLED_LCD-KeyMatrix-00.c
 *
 *  Created on: 2024/05/06
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
 *	 */

/*
 * 4x3�L�[�}�g���N�X
 *
 * A	PA10	CN10-33
 * B	PA9		CN10-21
 * C	PA8		CN10-23
 * D	PA7		CN10-15
 *
 * Z	PA6		CN10-13
 * Y	PA5		CN10-11
 * X	PA4		CN7-32
 *
 *
 */



/*
 *�@LCD���W���[���̔z��
 *
 * 	D7	PB15	CN10-26
 * 	D6	PB14	CN10-28
 * 	D5	PB13	CN10-30
 * 	D4	PB12	CN10-16
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




#include <stm32f10x.h>

//�֐��̃v���g�^�C�v�錾


void lcd_init(void);
void lcd_sendCommand(unsigned char cmd);
void lcd_sendData(unsigned char data);
void lcd_print(char * str);
void lcd_gotoxy(unsigned char x, unsigned char y);

void delay_us(uint16_t t);



void my_init_system(void);
void Wait_Time(int time);	//SysTick_Handler���g���҂����ԏ���

void SelectDigit(int digit);
void Display_7Seg(int number);



uint8_t keypad_kbhit();
uint8_t keypad_getkey();

#define DEBUG
#ifdef DEBUG
int COL;
#endif


//void Debug_Blink_OnBoard_LED(int , int);


#define LCD_RS	7
#define LCD_EN	6

#define DELAY	1000




#define UserButtonSW	(GPIOC->IDR)&(0x01<<13)



//�O���[�o���ϐ�
int TimeInt=0;	//delay�����iWait����Ԋu���w�肷�邽�߁j�̃O���[�o���ϐ��iSysTick�Ō��Z�j








//int main()
//{
//
//	my_init_system();//�n�[�h�E�F�A�̏�����
//
//	lcd_init();
//
//	lcd_gotoxy(1,1);				//move cursor to 1,1
//	lcd_print("The world is but");	//show "The world is but"
//	lcd_gotoxy(1,2);				//move cursor to 1,2
//	lcd_print("one country");		//show "one country"
//
//	Debug_Blink_OnBoard_LED(3, 500);
//	while(1);
//}














//
//���C���֐�
//

/*
 *  *���i���ɕ\������B
 *  �J�[�\���ʒu��1�����ÂE�ɓ������Ȃ���B
 *
 */


//int main(void){
//
//
//	my_init_system();
//	lcd_init();
//
//	while(1){
//	for(int line=1; line<=2; line++)
//		for(int col=1; col<=16; col++){
//			lcd_gotoxy(col,line);
//			lcd_sendCommand(0x0e);	//display on, cursor on
//			lcd_sendData('*');
//			Debug_Blink_OnBoard_LED(2,100);
//			Wait_Time(300);
//			lcd_sendCommand(0x01);
//		}
//
//
//	}
//}



///*
// * ABC�A�Ə��ԂɃA���t�@�x�b�g���t���ɕ\������B
// * Q�ȍ~��2�s�ڂɕ\������B
// *
// */
//
//int main(void){
//
//	my_init_system();
//	lcd_init();
//
//
//
//	lcd_gotoxy(1,1);
//
//	for(uint8_t val='A'; val<='Z'; val++){
//		if((val-'A')==16)	//Q�ȍ~��2�s�ڂɕ\������B
//			lcd_gotoxy(1,2);
//		lcd_sendData(val);
//		Debug_Blink_OnBoard_LED(2,100);
//
//	}
//
//	while(1)
//		Debug_Blink_OnBoard_LED(1,50);
//
//
//
//}






/*
 *
 *�@LCD�ɕ������\��
 *�@LCD�ɕ������\��7�Z�OLED�Ɉꌅ����0-3��\�����āA��������O���猅�V�܂ŌJ��Ԃ�
 *
 */


//int main()
//{
//
//	my_init_system();//�n�[�h�E�F�A�̏�����
//
//	loop:
//
//	lcd_init();
//
//	lcd_sendData(0x01);				//LCD clear
//	lcd_gotoxy(1,1);				//move cursor to 1,1
//	lcd_print("The world is but");	//show "The world is but"
//	lcd_gotoxy(1,2);				//move corsor to 1,2
//	lcd_print("one country");		//show "one country"
//
//
//	while(1){
//		for(int digit=0; digit<=7; digit++){
//			SelectDigit(digit);
//			for(int data=0; data<4; data++){
//				Display_7Seg(data);
//				Wait_Time(100);
//
//			}
//		}
//	}
//
//	goto loop;
//}





int main()
{

	my_init_system();//�n�[�h�E�F�A�̏�����

	lcd_init();
	lcd_gotoxy(1,1);	/* move cursor to 1,1 */
	lcd_print("The pressed key is: "); /* show the message */

	while(1)
	{




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
	/*�g�����ꂽ�A�r�b�g�Z�b�g�A���Z�b�g�̂������g���B*/
	GPIOB->ODR&=~0xf000;			//���4�r�b�g�̏�������
	GPIOB->ODR|=(value<<8)&0xf000;
	delay_us(DELAY);

	lcd_strobe();

	GPIOB->ODR&=~0xf000;			//����4�r�b�g�̏�������
	GPIOB->ODR|=(value<<12)&0xf000;

	lcd_strobe();
}


/*
 * LCD���W���[���ɑ΂��āA����R�}���h�𑗂�֐�
*/
void lcd_sendCommand(unsigned char cmd)
{
	GPIOB->ODR &= ~(1<<LCD_RS);	//RS =0 for command
	delay_us(DELAY);

	lcd_putValue(cmd);

}

/*
 * LCD���W���[���ɑ΂��āA�f�[�^(��ɕ����j�𑗂�֐�
*/

void lcd_sendData(unsigned char data)
{
	GPIOB->ODR |=(1<<LCD_RS);	//RS=1 for data
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




/* The function checks if a key is pressed or not */
/* Returns: 0: Not pressed, 1: Pressed */
uint8_t keypad_kbhit()
{
				   //FEDCBA9876543210
	GPIOA->ODR &= ~0b0000000001110000;		//A4,A5,A6��Low�ɂ���
	delay_us(1);
	if(((GPIOA->IDR>>8)&0xF) == 0xF) /* �����L�[��������Ă��Ȃ��ꍇ��0��Ԃ�*/
		return 0;
	else
		return 1;					/*����������Ă���΁A1��Ԃ�*/
}



/* The function returns the pressed key. */
/* It returns 0 if no key is pressed. */

uint8_t keypad_getkey()
{
						// ColX ColY ColZ
	uint8_t keypadLookup[]={'1','2','3',	//RowD
							'4','5','6',	//RowC
							'7','8','9',	//RowB
							'*','0','#'};	//RowA
	uint8_t character_code;

	const uint16_t colSelect[]={
		0x0010, /* ColX ZYX=001 */
		0x0020, /* ColY ZYX=010 */
		0x0040};/* ColZ ZYX=100 */

	if(keypad_kbhit() == 0)	/* if no key is pressed */
		return 0;

	for(int col = 0; col <= 2; col++) /* col 0 to 2 */
	{
		GPIOA->ODR &= ~colSelect[col]; /* �Z���N�g���Ă���񂾂���L�ɂ��� */
		delay_us(10);	/* wait for the columns to beupdated */
		uint8_t row = (GPIOA->IDR>>8)&0xF;

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
#ifdef DEBUG
		COL=col;
#endif
	}

	return 0; //�Ԃ�l��0�̏ꍇ�́A�����{�^���̉�������
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


/*
 * �f�o�b�O�p
 *
 */
void Debug_Blink_OnBoard_LED(int count, int delay)
{
	for(int i=0; i<count; i++){
		GPIOA->ODR^=(0x1<<5);
		Wait_Time(delay);
	}
}
