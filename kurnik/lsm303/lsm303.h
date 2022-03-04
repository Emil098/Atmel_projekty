/*
 * lsm303.h
 *
 *  Created on: 3 mar 2020
 *      Author: Emil
 */

#ifndef LSM303_LSM303_H_
#define LSM303_LSM303_H_

#include <avr/io.h>
#include <util/delay.h>

#include "D:\projekty eclipse\kurnik_1\i2c\i2c.h"

#define LM75_REG_TEMP 0x00 // Temperature Register
#define LM75_REG_CONF 0x01 // Configuration Register
#define LM75_ADDR 0b1001000 // LM75 address

void lm_init();
int lm_read();


#endif /* LSM303_LSM303_H_ */
