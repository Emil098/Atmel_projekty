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
//void eprom_writeAlarm(uint8_t day,uint8_t hour,uint8_t min){
//	uint8_t data = eprom_readFrom(eep_dayReg);
//	data |= (1<<(day-1));
//	eprom_writeTo(eep_dayReg,data);
//	eprom_writeTo(eep_min[day-1],min);
//	eprom_writeTo(eep_hour[day-1],hour);
//}
//uint8_t eeprom_checkAlarm(void){
//	uint8_t sec,hour,day,min,i,hourFromEEP,minFromEEP,ret=0;
//	day = ds_readSingleReg(dTReg);
//	day--;
//	i = eprom_readFrom(eep_dayReg);
//	if(((i>>day)&1)==1){
//		sec = ds_readSingleReg(secReg);
//		sec = (((sec>>4))*10)+(sec&0x0F);
//		hour = ds_readSingleReg(hourReg);
//		hour = (((hour>>4))*10)+(hour&0x0F);
//		min = ds_readSingleReg(minReg);
//		min = (((min>>4))*10)+(min&0x0F);
//		hourFromEEP = eprom_readFrom(eep_hour[day]);
//		minFromEEP = eprom_readFrom(eep_min[day]);
//		if(hour==hourFromEEP&&min==minFromEEP&&sec>15){
//			ret = 2;
//		}
//	}
//	return ret;
//}
//uint8_t eprom_check_summer_winter(void){
//	uint8_t czas = eprom_readFrom(dayReg);
//	czas = (czas>>7)&1;
//	return czas;
//}
//void eprom_del1Alarm(uint8_t day){
//	uint8_t data = eprom_readFrom(eep_dayReg);
//	data &= ~(1<<(day-1));
//	eprom_writeTo(eep_dayReg,data);
//}


