/*
 * menu.c

 *
 *  Created on: 2 lis 2017
 *      Author: Dv8
 */
#include "menu.h"
#include <avr/io.h>
#include <util/delay.h>
#include "../disp/disp.h"
#include "../ds1307/ds1307.h"
#include "../tim/tim.h"
#include "../eprom/eprom.h"


void menu_init(void){
	DDRB &= 0b10001100;
}
uint8_t menu_counter(uint8_t acc,uint8_t min,uint8_t max){
	int i=acc;
	if(acc<min)i=min;
	if(acc>max)i=max;
	if(!button_2){
		wait;i++;
		if(i==(max+1))i=min;
	}
	if(!button_3){
		wait;i--;
		if(i==min-1)i=max;
	}
	return i;
}
void menu_main(void){
	uint8_t menuON = 0;
	menuON=1;tim_reset();_delay_ms(200);
	uint8_t i=1;
	while(menuON){
		ds_setSQW;
		if(!button_1){
			_delay_ms(200);menuON=0;tim_set();
		}
		i = menu_counter(i,1,4);
		if(!button_4){
			_delay_ms(200);
			ds_SQWblink();
			if(i==1)menu_setTime();
			if(i==2)menu_setAlarm();
			if(i==3)menu_del1Alarm();
			if(i==4)menu_delAll();
			tim_set();
			menuON=0;
		}
		disp_set(1,i);
		disp_set(2,0);
		disp_set(3,empty);
		disp_set(4,0);
	}
}
void menu_setTime(void){
	uint8_t menuON = 1,i1=0,i2=0,c=0;
	uint8_t dataTab[7];
	while(menuON){
		if(!button_1){
			_delay_ms(200);menuON=0;
		}
		if(c==0)i1 = menu_counter(i1,0,59); //minuta
		if(c==1)i2 = menu_counter(i2,0,23); //godzina
		if(c==2)i1 = menu_counter(i1,1,7);  //dzieñ tygodnia
		if(c==3)i2 = 0;                     //pusty ekran dla dnia tygodnia
		if(c==4)i1 = menu_counter(i1,1,31); //dzieñ
		if(c==5)i2 = menu_counter(i2,1,12); //miesi¹c
		if(c==6)i1 = menu_counter(i1,0,99); //rok
		if(!button_4){
			ds_SQWblink();
			if(!(c%2)){
				dataTab[c]=i1;
				if(c==2)i1=0;
			}
			else{
				dataTab[c]=i2;
				i1=0;i2=0;
			}
			if(c==2)c=3;
			c++;
			_delay_ms(200);
			if(c==7){
				ds_setTime(0,dataTab[0],dataTab[1],dataTab[2],dataTab[4],dataTab[5],dataTab[6]);
				disp_done();menuON=0;
			}
		}
		disp_set(1,i1%10);
		disp_set(2,i1/10);
		if(c!=6)disp_set(3,i2%10);
		else disp_set(3,0);
		if(c!=6)disp_set(4,i2/10);
		else disp_set(4,2);
	}
}
void menu_setAlarm(void){
	uint8_t menuON = 1,i1=0,i2=0,c=0;
	uint8_t dataTab[3];
	while(menuON){
		if(!button_1){
			_delay_ms(200);menuON=0;
		}
		if(c==0)i1 = menu_counter(i1,0,59); //minuta
		if(c==1)i2 = menu_counter(i2,0,23); //godzina
		if(c==2)i1 = menu_counter(i1,1,7);  //dzieñ tygodnia
		if(!button_4){
			ds_SQWblink();
			if(!(c%2)){
				dataTab[c]=i1;
				if(c==2)i1=0;
			}
			else{
				dataTab[c]=i2;
				i1=0;i2=0;
			}
			c++;
			_delay_ms(200);
			if(c==3){
				eprom_writeAlarm(dataTab[2],dataTab[1],dataTab[0]);
				disp_done();menuON=0;
			}
		}
		disp_set(1,i1%10);
		disp_set(2,i1/10);
		disp_set(3,i2%10);
		disp_set(4,i2/10);
	}
}
void menu_del1Alarm(void){
	uint8_t menuON = 1,i=1;
	while(menuON){
		if(!button_1){
			_delay_ms(200);menuON=0;
		}
		i = menu_counter(i,1,7); //dzieñ tygodnia
		if(!button_4){
			_delay_ms(200);
			ds_SQWblink();
			eprom_del1Alarm(i);
			disp_done();menuON=0;
		}
	disp_set(1,i);
	disp_set(2,0);
	disp_set(3,0);
	disp_set(4,0);
	}
}
void menu_delAll(void){
	uint8_t i;
	for(i=1;i<8;i++)eprom_del1Alarm(i);
	disp_done();
}

