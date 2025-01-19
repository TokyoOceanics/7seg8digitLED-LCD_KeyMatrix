/*
 * lcd_02.c
 *
 *  Created on: Jly 5, 2016
 *      Author: gizmy
 */
// LCD Test Program : 使用ポートは４つ。
//
// LCD DB4-7 : PA8-11
//			DB7 PA11 CN10-14
//			DB6 PA10 CN10-33
//			DB5 PA09 CN10-21
//-			DB4 PA08 CN10-23
//
// 		   LCD STM
// LCD RS: 4   PB0  Register Select	CN7-34
// LCD EN: 5   PB1  Enable			CN10-24
// LCD RW: 6   PB2  Read/Write 常時WRITEで使うので、GND接続でも可。

// SWO-1: PB1-2

#include <stm32f10x.h>
/*
#define RCC_APB2ENR	  (*(unsigned long*) 0x40021018)

#define GPIOA_CRL     (*(unsigned long*) 0x40010800)
#define GPIOA_CRH     (*(unsigned long*) 0x40010804)
#define GPIOA_IDR     (*(unsigned long*) 0x40010808)
#define GPIOA_ODR     (*(unsigned long*) 0x4001080c)

#define GPIOB_CRL     (*(unsigned long*) 0x40010c00)
#define GPIOB_IDR     (*(unsigned long*) 0x40010c08)
#define GPIOB_ODR     (*(unsigned long*) 0x40010c0c)

//#define GPIOC_CRL     (*(unsigned long*) 0x40011000)
//#define GPIOC_CRH     (*(unsigned long*) 0x40011004)
//#define GPIOC_IDR     (*(unsigned long*) 0x40011008)
//#define GPIOC_ODR     (*(unsigned long*) 0x4001100c)
*/
//  ----- システム定数
                    // LCD  STM
                    // PIN  PIN
#define RS	 0x01	//  4   PB0  Register Select 		CN7-34
#define EN	 0x02	//  5   PB1  Enable					CN10-24
#define RW   0x04	//  6   PB2  Read/Write 常時WRITE

#define TIMCNT	2200  // case 8MHz CLK for sleep1ms()

void InitPort(void);
void sleep1ms(int ms);
void lcd_init(void);
void lcd_putc(char c);
void set_bit(int bit);
void reset_bit(int bit);
void data_write(char c);
void data2_write(char c);

void lcd_puts(char *s);
void locate_line(int line);
void cursor_blink(int curs, int blnk);
void clear_display(void);

int main(void)
{
	InitPort();
    lcd_init();

    locate_line(1);		//1行目先頭へ
    cursor_blink(1,0);
  	lcd_putc('A');
	lcd_putc('B');
	lcd_putc('C');

    locate_line(2);		//2行目先頭へ
    cursor_blink(1,1);
    lcd_putc('x');
    lcd_putc('y');
    lcd_putc('z');

    locate_line(1);		//1行目先頭へ
    cursor_blink(0,1);
    lcd_putc('1');
    lcd_putc('2');
    lcd_putc('3');

    locate_line(2);		//2行目の先頭へ
    cursor_blink(0,0);
    lcd_putc('ｱ');
    lcd_putc('ｲ');
    lcd_putc('ｳ');

    clear_display();
    lcd_puts("ABCabc123ｱｲｳ");

    locate_line(2);
    lcd_puts("Winatelier Ltd.");

	while (1)
		;
}

void clear_display(void)
{
	reset_bit(RS);	   //制御レジスタにｾｯﾄ＝コマンドモードにｾｯﾄ

	data2_write(0x01); //display clear
	sleep1ms(2);	   //1.64ms

    cursor_blink(1,0);
    locate_line(1);
}

void lcd_puts(char *s)
{
	while(*s) {
		lcd_putc(*s);
		s++;
	}
}

//引数 :1:１行目、2:２行目
void locate_line(int line)
{
	unsigned char cmd[2]={0x80, 0xC0};

    reset_bit(RS);		//制御レジスタにｾｯﾄ＝コマンドモードにｾｯﾄ

	data2_write(cmd[line-1]);	//line行目先頭へ
	sleep1ms(1);
}

//引数：curs:0/1=off/on、blink:0/1=off/on
void cursor_blink(int curs, int blnk)
{
	unsigned char cmd;

    reset_bit(RS);	//制御レジスタにｾｯﾄ＝コマンドモードにｾｯﾄ

    //DB4    DB3     DB2    DB1
	//1      1       1      0   <--0x0E
	//CMD   /display/cursor/blink
    cmd = 0x0C;
	if (curs)
		cmd |= 0x02;
	else
		cmd &= ~0x02;

	if (blnk)
		cmd |= 0x01;
	else
		cmd &= ~0x01;

	data2_write(cmd);
	sleep1ms(1);
}

void set_bit(int bit)
{
	GPIOB->ODR |= bit;
	sleep1ms(1);
}

void reset_bit(int bit)
{
	GPIOB->ODR &= ~bit;
	sleep1ms(1);
}

void data_write(char data)
{
	GPIOA->ODR = (data & 0x0f) << 8;
    set_bit(EN);      // 0    1    0   E up
	reset_bit(EN);    // 0    0    0   E down
}

void data2_write(char data)
{
	GPIOA->ODR = (data & 0xf0) << 4;
    set_bit(EN);      // 0    1    0   E up
	reset_bit(EN);    // 0    0    0   E down

	GPIOA->ODR = (data & 0x0f) << 8;
    set_bit(EN);      // 0    1    0   E up
	reset_bit(EN);    // 0    0    0   E down
}

void lcd_init(void)
{
    GPIOB->ODR = 0x00;  // Command mode
	sleep1ms(15);      // 1 - 15ms

	data_write(0x03);  // 2 - 8bit mode
	sleep1ms(5);       // 3 - 4.1ms

	data_write(0x03);  // 4 -
	sleep1ms(1);       // 5 - 100us

	data_write(0x03);  // 6 -
	sleep1ms(1);       // 7 - 40us

    data_write(0x02);  // 8 - 4bit mode
	sleep1ms(1);	   // 9 - 40us

	//ここからは待ち時間を使わないで、Busyフラグを使ってタイミングをとっても良い。

    data2_write(0x28); //10 - function set/ 2行表示モード
	sleep1ms(1);	   //11 - 40us

	                   //DB4  DB3     DB2    DB1
	                   // 1   0       0      0   <--0x08
	data2_write(0x08); //12 - display/cursor/blink off
	sleep1ms(1);	   //13 - 40us

	data2_write(0x01); //14 - display clear
	sleep1ms(2);	   //15 - 1.64ms

	data2_write(0x06); //16 - Entry mode, cursor moving direction:right
	sleep1ms(1);       //17 - 40us

	//ここまでで初期化は完了。
    				   //DB4  DB3     DB2    DB1
    				   // 1   1       1      0   <--0x0E
    data2_write(0x0f); //18 - display/cursor on blink off
    sleep1ms(1);
}

void lcd_putc(char c)
{
	                // RS EN  R/W
	set_bit(RS);    // 1   0   0  <--データレジスタにｾｯﾄ＝表示モード

	data2_write(c);
	sleep1ms(1);
}

void sleep1ms(int ms)
{
	int i, j;

	for (j=0; j < ms; j++) {
		for (i=0; i < TIMCNT; i++)
			;
	}
}

void InitPort(void)
{
	RCC->APB2ENR |= (unsigned long) 0x000C;  // Enable C/B/A/-/AFIO
											//        0 1 1 0 0  <--0x0C
	GPIOA->CRH = 0x44442222;       //PA8-11:DB4-7

	GPIOB->CRL = 0x44444222;       //PB0-2:RS,EN,R/W

	//GPIOB_ODR = 0x00000001;
	//GPIOA_ODR = 0x000000ff;
}


