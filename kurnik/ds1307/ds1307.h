/*
 * ds1307.h
 *
 *  Created on: 26 paü 2017
 *      Author: Dv8
 */

#ifndef DS1307_DS1307_H_
#define DS1307_DS1307_H_

#include <avr/io.h>
#include <util/delay.h>
#include "D:\projekty eclipse\kurnik_1\i2c\i2c.h"

#define ds_addr 0b1101000
#define ds_setSQW PORTC |= (1<<PC7);
#define ds_resetSQW PORTC &= ~(1<<PC7);

#define secReg 0
#define minReg 1
#define hourReg 2
#define dTReg 3
#define dayReg 4
#define mounthReg 5
#define yearReg 6

void ds_init(void);
void ds_setTime(uint8_t sec,uint8_t  min,uint8_t  hour,uint8_t dayOfWeek,uint8_t day,uint8_t mounth,uint8_t  year);
void ds_setSingleReg(uint8_t regNr,uint8_t data);
uint8_t ds_readSingleReg(uint8_t  regNr);
int ds_readTime(void);
int ds_readDate(void);
void ds_enableSQW(void);
void ds_checkSQW(void);
void ds_SQWblink(void);

#endif /* DS1307_DS1307_H_ */
