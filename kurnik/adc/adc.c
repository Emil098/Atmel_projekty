/*

 * adc.c
 *
 *  Created on: 13 lis 2017
 *      Author: Dv8
 */
#include <avr/io.h>
#include <util/delay.h>
#include "adc.h"


void adc_init(void){
	ADCSRA = 0;
	ADMUX = 0;
	ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);  //adc 0,Vref odniesienie
	//W³¹czenie ADC, prescaler 128
}
uint16_t adc_getdata(void){
	ADCSRA |= (1<<ADSC);
	uint8_t isItNow=1;
	while(!isItNow){
		isItNow = ((ADCSRA>>ADSC)&1);
	}
	uint16_t val = (ADCL|(ADCH<<8));
	return val;
}
uint16_t adc_getVolt(void){
	return map(adc_getdata(),1007,729,1502,1088);
}
int map(long x, long in_min, long in_max, long out_min, long out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
