/*
 * 20240413-7SegLED_and_LCD-01.c
 *
 *  Created on: 2024/04/13
 *      Author: x-lib
 *
 *
 *�@LCD���W���[����7SegLED�~8���\����̓���m�F�v���O����
 *
 *�@�͌��搶�̃{�[�h�𗬗p����
 *
 *	LCD�\�����䂪���܂��������A���낢��Y�ށB
 *	lcd_putValue�֐��ŁABRR�ABSRR���g���ƕs����N����B�������2�̃��W�X�^�𗝉��ł��Ă��Ȃ��B
 *	���A�b�Ńr�b�g�����Z�b�g�A�Z�b�g��������ŁA���삷��悤�ɂȂ����B
 *	LCD_strobe�֐��ŁA�`�����{�����Ă��āA�C���t���̂ɔ�����v�����B
 *
 *	�z�������������߁A�z���~�X���Y�܂����󋵂��Ă�
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

void Debug_Blink_OnBoard_LED(int , int);


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









int main()
{

	my_init_system();//�n�[�h�E�F�A�̏�����


	lcd_init();

loop:
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
				Debug_Blink_OnBoard_LED(2,200);
			}
		}
	}

	goto loop;
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
#if 0	/*���̃u���b�N�́A���낢�뎎�������A���܂����삵�Ȃ�*/
//	uint16_t debug_value;

	//value�̏��4�r�b�g��PB15-PB12�iLCD��D7-D4�j�ɏo�͂���
	//
	GPIOB->BRR=0xf0000000;	//PB15-PB12���[���N���A����
	//                       3210
	GPIOB->BSRR=(value<<8)&0xf000;	//value�̏��4�r�b�g�� PB15-PB12�֏o��
//	debug_value=(value<<8)&0xf000;
	delay_us(DELAY);

	lcd_strobe();

	//value�̉���4�r�b�g�f�[�^��PB15-PB12�iLCD��D7-D4�j�ɏo�͂���
	GPIOB->BRR=0xf0000000;	//PB15-PB12���[���N���A����
	GPIOB->BSRR=(value<<12)&0xf000;	//value�̉��ʂS�r�b�g��PB15-PB12�֏o��
//	debug_value=(value<<12)&0xf000;

	delay_us(DELAY);

	lcd_strobe();
#else
	/*�g�����ꂽ�A�r�b�g�Z�b�g�A���Z�b�g�̂������g���B*/
	GPIOB->ODR&=~0xf000;			//���4�r�b�g�̏�������
	GPIOB->ODR|=(value<<8)&0xf000;
	delay_us(DELAY);

	lcd_strobe();

	GPIOB->ODR&=~0xf000;			//����4�r�b�g�̏�������
	GPIOB->ODR|=(value<<12)&0xf000;

	lcd_strobe();

#endif


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

#if 0
		//�R�c�搶�̏������V�[�P���X
		//0x30 -> 0x30 -> 0x30 -> 0x20
		lcd_sendCommand(0x33);	//8bit mode 7654
								//          0011
		lcd_sendCommand(0x32);	//functionSet 4bit

		lcd_sendCommand(0x28);	//functionSet 4bit, 2line, 5x7font�@�璷�Ȃ���A4�r�b�g���[�h������x���{


		lcd_sendCommand(0x08);	//dispTurnOff, cursorOff, blinkOff
		lcd_sendCommand(0x01);	//dispClear
		lcd_sendCommand(0x06);	//entryMode cursorMoveRight
		lcd_sendCommand(0x0f);	//dispTurnOn

#else


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


#endif

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

	//GPIO�@B,C��L����
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN|RCC_APB2ENR_IOPCEN;	// GPIOB,C�̃N���b�N��L���ɂ���

	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;	//GPIOA �f�o�b�O�p�ɗL����

	//TIM3 �L�������āAPB0��TIM3�Ƃ���
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;	//TIM3�̗L����

	//�|�[�gA�@�f�o�b�O�p
	//			 76543210
	GPIOA->CRL=0x44144444;



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
