/*
 * 7Segment_LED_Control.c
 *
 *  Created on: 2024/06/09
 *      Author: x-lib
 */

extern DispData[];

//７segLEDの表示データ、負論理
int seg7[]={0xc0, 0xf9, 0xa4,0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90};



int segment_control(){
	int state;	//点灯制御の状態
	static int state_count=0;
	static int segment_digit;	//表示桁

	switch (state){
		case 0:			//digit 0
			state=1;
			segment_digit=0;
			break;
		case 1:
			state=2;
			segment_digit=0;
			break;
		case 2:
			state=3;
			segment_digit=0;
			break;
		case 3:
			state=4;
			segment_digit=OFF;
			break;
		case 4:			//digit 1
			state=5;
			segment_digit=1;
			break;
		case 5:
			state=6;
			segment_digit=1;
			break;
		case 6:
			state=7;
			segment_digit=1;
			break;
		case 7:
			state=8;
			segment_digit=OFF;
			break;
		case 4:			//digit 2
			state=5;
			segment_digit=2;
			break;
		case 5:
			state=6;
			segment_digit=2;
			break;
		case 6:
			state=7;
			segment_digit=2;
			break;
		case 7:
			state=8;
			segment_digit=OFF;
			break;

		case 8:			//digit 3
			state=9;
			segment_digit=3;
			break;
		case 9:
			state=10;
			segment_digit=3;
			break;
		case 10:
			state=11;
			segment_digit=3;
			break;
		case 11:
			state=12;
			segment_digit=OFF;
			break;


		case 12:			//digit 4
			state=13;
			segment_digit=4;
			break;
		case 13:
			state=14;
			segment_digit=4;
			break;
		case 14:
			state=15;
			segment_digit=4;
			break;
		case 15:
			state=16;
			segment_digit=OFF;
			break;

		case 16:			//digit 5
			state=17;
			segment_digit=5;
			break;
		case 17:
			state=18;
			segment_digit=5;
			break;
		case 18:
			state=19;
			segment_digit=5;
			break;
		case 19:
			state=20;
			segment_digit=OFF;
			break;

		case 20:			//digit 6
			state=21;
			segment_digit=5;
			break;
		case 21:
			state=22;
			segment_digit=5;
			break;
		case 22:
			state=23;
			segment_digit=5;
			break;
		case 23:
			state=24;
			segment_digit=OFF;
			break;

		case 24:			//digit 6
			state=25;
			segment_digit=6;
			break;
		case 25:
			state=26;
			segment_digit=6;
			break;
		case 26:
			state=27;
			segment_digit=6;
			break;
		case 27:
			state=28;
			segment_digit=OFF;
			break;

		case 28:			//digit 7
			state=29;
			segment_digit=6;
			break;
		case 29:
			state=30;
			segment_digit=6;
			break;
		case 30:
			state=31;
			segment_digit=6;
			break;
		case 31:
			state=0;
			segment_digit=OFF;
			break;

		default:





	}



}




void SegmentControl(void){


	u_int8_t digit_on_off;
}
