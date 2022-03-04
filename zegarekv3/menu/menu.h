/*
 * menu.h
 *
 *  Created on: 2 lis 2017
 *      Author: Dv8
 */

#ifndef MENU_MENU_H_
#define MENU_MENU_H_
#include <avr/io.h>
#include <util/delay.h>
#include "../disp/disp.h"
#include "../ds1307/ds1307.h"
#include "../tim/tim.h"

// volatile uint8_t menuON = 0;
//extern volatile uint8_t menuON = 0;

#define button_5 ((PINB>>PB6)&1)
#define button_4 ((PINB>>PB5)&1)
#define button_3 ((PINB>>PB4)&1)
#define button_2 ((PINB>>PB1)&1)
#define button_1 ((PINB>>PB0)&1)
#define wait _delay_ms(200);

void menu_init(void);
void menu_main(void);
void menu_setTime(void);
void menu_setAlarm(void);
void menu_del1Alarm(void);
void menu_delAll(void);
uint8_t menu_counter(uint8_t acc,uint8_t min,uint8_t max);

#endif /* MENU_MENU_H_ */
