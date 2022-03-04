/*

 * adc.h
 *
 *  Created on: 13 lis 2017
 *      Author: Dv8
 */

#ifndef ADC_ADC_H_
#define ADC_ADC_H_
#include <avr/io.h>
#include <util/delay.h>

void adc_init(void);
uint16_t adc_getdata(void);
uint16_t adc_getVolt(void);
int map(long x, long in_min, long in_max, long out_min, long out_max);



#endif /* ADC_ADC_H_ */
