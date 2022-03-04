/*
 * eprom.h
 *
 *  Created on: 6 lis 2017
 *      Author: Dv8
 */

#ifndef EPROM_H_
#define EPROM_H_
#include <avr/io.h>
#include <util/delay.h>
//#include "../ds1307/ds1307.h"

//#define eep_dayReg 0  //w kom�rce 0 epromu przechowane s� dni (0-6) w ktore jest aktywny alarm
//static uint8_t eep_min[7] = {1,3,5,7,9,11,13};  //rejetry w kt�rych zapisywane s� godz,min dla kolejnych dni tyg 0-6
//static uint8_t eep_hour[7] = {2,4,6,8,10,12,14};

void eprom_writeTo(uint16_t addr,uint8_t data);
//void eprom_writeAlarm(uint8_t day,uint8_t hour,uint8_t min);
//uint8_t eeprom_checkAlarm(void);
//uint8_t eprom_check_summer_winter(void);
uint8_t eprom_readFrom(uint16_t addr);
//void eprom_del1Alarm(uint8_t day);


#endif /* EPROM_H_ */
