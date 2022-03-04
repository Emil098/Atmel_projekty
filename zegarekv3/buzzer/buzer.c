/*

 * buzer.c
 *
 *  Created on: 8 lis 2017
 *      Author: Dv8
 */

#include <avr/io.h>
#include <util/delay.h>
#include "buzzer.h"

void buzzer_init(void){
	SREG &= ~(1<<7);
	TCCR1A = 0;
	TCCR1B = 0;
#if(CPU_freq==CPU_freq_16)
		//	TCCR0A = 0;
		//	TCCR0B = 0;
		//	TCCR0A |= (1<<WGM01);
		//	TCCR0B |= (1<<CS00)|(1<<CS01);     //  ATMEGA328
		//	uint8_t val = 0x7C;
		//	val *= 2;
		//	OCR0A =val;
		//	TCNT0 = 0;
		//	TIMSK0 |= (1<<OCIE0A);
#endif
 #if (CPU_freq==CPU_freq_8)
		TCCR0 = 0;
		TCCR0 |= (1<<WGM01)|(1<<CS00)|(1<<CS01);
		uint8_t val = 0x7C;
		OCR0 =val;
		TCNT0 = 0;
		TIMSK |= (1<<OCIE0);
#endif
	TCCR1B |= (1<<WGM12);
	TCCR1B |= presc_1;
	TCNT1 = 0;
//	uint16_t val = 0x0C7F;  //5kHz
//	val /= 2;
//	OCR1A = val;
//	TIMSK |= (1<<OCIE1A);
	SREG |= (1<<7);
}

void buzzer_setFreq(uint32_t freq){
	TCCR1B &= ~(0b111);  // reset preskalera
	uint16_t presc;
	if(freq>250){
		presc = 1;TCCR1B |= presc_1;
	}
	else{
		presc = 8;TCCR1B |= presc_8;
	}
	uint16_t val = get_OCR_val(freq,presc);
	val /= 2;
	OCR1A = val;
	//	TCNT1L = 0;
	//	TCNT1H = 0;
}

void buzzer_dur(uint8_t time_ms){

}
uint16_t get_OCR_val(uint32_t freq,uint16_t presc){
	return ((CPU_freq / (presc * freq)) - 1);
}
