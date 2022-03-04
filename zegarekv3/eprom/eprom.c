/*
 * eprom.c
 *
 *  Created on: 6 lis 2017
 *      Author: Dv8
 */
#include <avr/io.h>
#include <util/delay.h>
#include "eprom.h"
#include "../ds1307/ds1307.h"


void eprom_writeTo(uint16_t addr,uint8_t data){
	while(((EECR>>EEWE)&1));
	EEARL = addr&0xFF;
	EEARH = (addr>>8)& 0b11;
	EEDR = data;
	EECR |= (1<<EEMWE);
	EECR |= (1<<EEWE);
}
uint8_t eprom_readFrom(uint16_t addr){
	while(((EECR>>EEWE)&1));
	EECR = 0;
	EEARL= addr&0xFF;
	EEARH = (addr>>8)& 0b11;
	EECR |= (1<<EERE);
	while((EECR>>EERE)&1);
	return EEDR;
}
void eprom_writeAlarm(uint8_t day,uint8_t hour,uint8_t min){
	uint8_t data = eprom_readFrom(eep_dayReg);
	data |= (1<<(day-1));
	eprom_writeTo(eep_dayReg,data);
	eprom_writeTo(eep_min[day-1],min);
	eprom_writeTo(eep_hour[day-1],hour);
}
back_v eeprom_checkAlarm(void){
	uint8_t hour,day,min,sec,i,hourFromEEP,minFromEEP,ret=0;
	back_v rr = {0};
	day = ds_readSingleReg(dTReg);
	day--;
	rr.dayreg = day;
	i = eprom_readFrom(eep_dayReg);
	i &= 0b1111111; // bo w epp_dayreg na ostatnim bicie jest czas letni/zimowy
	rr.dayreg_ep = i;
	if((i>>day)&1){
		min = ds_readSingleReg(minReg);
		min = (((min>>4)&0x0F)*10)+(min&0x0F);
		rr.Min = min;
		hour = ds_readSingleReg(hourReg);
		hour = (((hour>>4)&0x0F)*10)+(hour&0x0F);
		rr.Hour = hour;
		sec = ds_readSingleReg(secReg);
		sec = (((sec>>4)&0x0F)*10)+(sec&0x0F);
		rr.Sec = sec;
		hourFromEEP = eprom_readFrom(eep_hour[day]);
		rr.Hour_ep = hourFromEEP;
		minFromEEP = eprom_readFrom(eep_min[day]);
		rr.Min_ep = minFromEEP;
		if(hour==hourFromEEP&&min==minFromEEP&&sec==30){
			rr.ret = 1;
		}
	}
	return rr;
}
void eprom_del1Alarm(uint8_t day){
	uint8_t data = eprom_readFrom(eep_dayReg);
	data &= ~(1<<(day-1));
	eprom_writeTo(eep_dayReg,data);
}


