/*
 * tim.h
 *
 *  Created on: 2 lis 2017
 *      Author: Dv8
 */

#ifndef TIM_H_
#define TIM_H_

#include <avr/io.h>
#include <util/delay.h>
//#include <avr/interrupt.h>
#  define ISR(vector, ...)            \
		void vector (void) __attribute__ ((signal,__INTR_ATTRS)) __VA_ARGS__; \
		void vector (void)


void tim_init(void);
void tim_reset(void);
void tim_set(void);

#endif /* TIM_H_ */
