/*
 * hc05.h
 *
 *  Created on: 19 gru 2017
 *      Author: Dv8
 */

#ifndef HC05_HC05_H_
#define HC05_HC05_H_
#include <avr/io.h>
#include <util/delay.h>
#include "../uart/uart.h"

uint8_t state=0;
uint16_t counter=0,data[3];

void HC05_begin();
uint16_t HC05_recive();
void HC05_send(uint16_t value);

#endif /* HC05_HC05_H_ */
