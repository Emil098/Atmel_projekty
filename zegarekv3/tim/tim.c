/*
 * tim.c

 *
 *  Created on: 2 lis 2017
 *      Author: Dv8
 */
#include <avr/io.h>
#include <util/delay.h>

void tim_init(void){
	SREG &= ~(1<<7);
	TCCR2 = 0;
	TCCR2 |= (1<<WGM21);  //CTC
	TCCR2 |= (1<<CS20)|(1<<CS21)|(1<<CS22);  //PRESCALER 1024
	OCR2 = 0xC2;   //25 ms
	TCNT2 = 0;
	TIMSK |= (1<<OCIE2); // w³¹cza przerwanie OCR2
	SREG |= (1<<7);  // w³¹cza globalne przerwania
}
void tim_reset(void){
	SREG &= ~(1<<7);
	TCNT2 = 0;
}
void tim_set(void){
	SREG |= (1<<7);
}

