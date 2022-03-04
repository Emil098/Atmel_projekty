/*
 * ds1307.c
 *
 *  Created on: 26 paü 2017
 *      Author: Dv8
 */
#include "ds1307.h"
#include <avr/io.h>
#include <util/delay.h>
#include "../i2c/i2c.h"

void ds_init(void){
	i2c_init();
	ds_enableSQW();
}

void ds_setTime(uint8_t sec,uint8_t  min,uint8_t  hour,uint8_t dayOfWeek,uint8_t day,uint8_t mounth,uint8_t  year){ //rok to liczba 2 cyfrowa znaczy 17
	sec = ((sec/10)<<4)|(sec%10);
	min = ((min/10)<<4)|(min%10);
	hour = ((hour/10)<<4)|(hour%10);
	hour &= ~(0b11<<6);
	dayOfWeek &= 7;
	day = ((day/10)<<4)|(day%10);
	mounth = ((mounth/10)<<4)|(mounth%10);
	year = ((year/10)<<4)|(year%10);
	i2c_start();
	i2c_sendTo(ds_addr);
	i2c_send_byte(0);
	i2c_send_byte(sec);
	i2c_send_byte(min);
	i2c_send_byte(hour);
	i2c_send_byte(dayOfWeek);
	i2c_send_byte(day);
	i2c_send_byte(mounth);
	i2c_send_byte(year);
	i2c_stop();

}
void ds_setSingleReg(uint8_t regNr,uint8_t data){
	i2c_start();
	i2c_sendTo(ds_addr);
	i2c_send_byte(regNr);
	i2c_send_byte(data);
	i2c_stop();
}
uint8_t ds_readSingleReg(uint8_t  regNr){
	i2c_start();
	i2c_sendTo(ds_addr);
	i2c_send_byte(regNr);
	i2c_start();
	i2c_requestFrom(ds_addr);
	uint8_t ret=i2c_read_byte(0);

	return ret;
}
uint16_t ds_readTime(void){  //starsze 8 bitÛw godzina, m≥odsze 8 bitÛw minuty
	uint8_t min = ds_readSingleReg(minReg);
	min = (((min>>4)&0x0F)*10)+(min&0x0F);
	uint8_t hour = ds_readSingleReg(hourReg);
	hour = (((hour>>4)&0x0F)*10)+(hour&0x0F);
	uint16_t ret = (hour<<8)|min;
	return ret;
}
int ds_readDate(void){  //starsze 8 bitÛw miesiπc, m≥odsze 8 bitÛw dzieÒ
	uint8_t day = ds_readSingleReg(dayReg);
	day = (((day>>4)&0x0F)*10)+(day&0x0F);
	uint8_t mon = ds_readSingleReg(mounthReg);
	mon = (((mon>>4)&0x0F)*10)+(mon&0x0F);
	int ret = (mon<<8)|day;
	return ret;
}
void ds_enableSQW(void){
	DDRC |= (1<<PC7);
	PORTC |= (1<<PC7);
	DDRB &= ~(1<<PB2);
	i2c_start();
	i2c_sendTo(ds_addr);
	i2c_send_byte(7); // adres dla "control register"
	i2c_send_byte(0b00010000); // czÍstotliwoúc wyjúciowa 1 Hz
	i2c_stop();
}
void ds_checkSQW(void){
	if((PINB>>PB2)&1)PORTC |= (1<<PC7); //sprawdza pors sqw z ds1307 i ustawia diode na wyúwietlaczu
	else PORTC &= ~(1<<PC7);
}
void ds_SQWblink(void){
	PORTC |= (1<<PC7);
	_delay_ms(50);
	PORTC &= ~(1<<PC7);
	_delay_ms(50);
	PORTC |= (1<<PC7);
}
