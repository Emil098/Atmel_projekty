/*
 * menu.h
 *
 *  Created on: 2 mar 2020
 *      Author: Emil
 */

#ifndef MENU_MENU_H_
#define MENU_MENU_H_

#include <avr/io.h>
#include <util/delay.h>
#include "D:\projekty eclipse\kurnik_1\lcd\lcd.h"
#include "D:\projekty eclipse\kurnik_1\adc\adc.h"
#include "D:\projekty eclipse\kurnik_1\ds1307\ds1307.h"

// przyciski wewnetrzne
#define INT_BUTTON_REG DDRA
#define INT_BUTTON_1 PA6
#define INT_BUTTON_4 PA7
#define INT_BUTTON_2 PA5
#define INT_BUTTON_3 PA4

#define INT_BUTTON_1_ST (!((PINA>>PA6)&1))
#define INT_BUTTON_4_ST (!((PINA>>PA7)&1))
#define INT_BUTTON_2_ST (!((PINA>>PA5)&1))
#define INT_BUTTON_3_ST (!((PINA>>PA4)&1))

// przyciski zewnetrzne
#define EXT_BUTTON_REG DDRB
#define EXT_BUTTON_1 PB3
#define EXT_BUTTON_2 PB5
#define EXT_BUTTON_3 PB6
#define EXT_BUTTON_1_ST (!((PINB>>PB3)&1))
#define EXT_BUTTON_2_ST (!((PINB>>PB5)&1))
#define EXT_BUTTON_3_ST (!((PINB>>PB6)&1))


#define LED_LCD_OFF PORTB &= ~(1<<PB7)
#define LED_LCD_ON PORTB |= (1<<PB7)
#define MENU_LED_OFF PORTB |= (1<<PB0)
#define MENU_LED_ON PORTB &= ~(1<<PB0)
#define actuator_delay 45000
#define MENU_LED_BLINK MENU_LED_ON;_delay_ms(200);MENU_LED_OFF;_delay_ms(200)
#define PWM_LED_OFF PORTB |= (1<<PB2)
#define PWM_LED_ON PORTB &= ~(1<<PB2)

#define SLEEP_1 PORTA |= (1<<PA2);PORTC |= (1<<PC7)
#define SLEEP_0 PORTA &= ~(1<<PA2);PORTC &= ~(1<<PC7)
#define PHASE_1 PORTA |= (1<<PA3)
#define PHASE_0 PORTA &= ~(1<<PA3)
#define DOOR_LED_OFF PORTB |= (1<<PB1)
#define DOOR_LED_ON PORTB &= ~(1<<PB1)

uint8_t set_date();
void show_adc();
uint8_t service_menu();
uint8_t check_buttons();
void close_door();
void open_door();

#endif /* MENU_MENU_H_ */
