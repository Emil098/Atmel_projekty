/*
 * disp.c
 *
 *  Created on: 30 paü 2017
 *      Author: Dv8
 */
#include "disp.h"
#include <avr/io.h>
#include <util/delay.h>
#include "../ds1307/ds1307.h"


void disp_init(void){
	digit1DDR = 0xFF;
	digit2DDR = 0xFF;
	digit3DDR = 0xFF;
	digit4DDR = 0xFF;
}
void disp_set(uint8_t dig,uint8_t Nr){
//	Nr &= 9;
	if(dig==1||dig==2){
		dig--;
		uint8_t i;
		for(i=0;i<4;i++){
			if((nr_tab[Nr]>>i)&1)digit1Port |= (1<<digit[dig][i]);    // lampy na porcie a
			else digit1Port &= ~(1<<digit[dig][i]);
		}
	}
	else{
		dig--;
		uint8_t i;
		for(i=0;i<4;i++){
			if((nr_tab[Nr]>>i)&1)digit3Port |= (1<<digit[dig][i]);    // lampy na porcie d
			else digit3Port &= ~(1<<digit[dig][i]);
		}
	}
}
void disp_2_val(uint8_t val1,uint8_t val2){
	disp_set(1,val2%10);
	disp_set(2,val2/10);
	disp_set(3,val1%10);
	disp_set(4,val1/10);
}
void disp_year(void){
	uint8_t year =  ds_readSingleReg(yearReg);
	year = (((year>>4)&0x0F)*10)+(year&0x0F);
	disp_set(1,year%10);
	disp_set(2,year/10);
	disp_set(3,0);
	disp_set(4,2);
}
void disp_time(void){
	int dsTime = ds_readTime();
	int h=(dsTime>>8)&0xFF;
	int m=dsTime&0xFF;
	disp_set(1,m%10);
	disp_set(2,m/10);
	disp_set(3,h%10);
	disp_set(4,h/10);
}
void disp_date(void){
	int dsDate = ds_readDate();
	int mon=(dsDate>>8)&0xFF;
	int day=dsDate&0xFF;
	disp_set(1,day%10);
	disp_set(2,day/10);
	disp_set(3,mon%10);
	disp_set(4,mon/10);
}
void disp_done(void){
	uint8_t i=1;
	disp_empty();
	for(i=1;i<5;i++){
		disp_set(i,1);
		_delay_ms(200);
		disp_empty();
	}
}
void disp_done_0(void){
	uint8_t i=1;
	disp_empty();
	for(i=1;i<5;i++){
		disp_set(i,0);
		_delay_ms(200);
		disp_empty();
	}
}
void disp_empty(void){
	uint8_t c=1;
	for(c=1;c<5;c++){
		disp_set(c,10);
	}
}
void disp_one_digit(uint8_t nr){
	uint8_t c=1;
	for(c=2;c<5;c++){
		disp_set(c,10);
	}
	disp_set(1,nr);
}

