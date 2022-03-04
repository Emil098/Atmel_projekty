/*
 * disp.h
 *
 *  Created on: 30 paü 2017
 *      Author: Dv8
 */

#ifndef DISP_H_
#define DISP_H_

#include <avr/io.h>
#include <util/delay.h>

#define digit1Port PORTA
#define digit1DDR DDRA
#define digit2Port PORTA
#define digit2DDR DDRA
#define digit3Port PORTD
#define digit3DDR DDRD
#define digit4Port PORTD
#define digit4DDR DDRD
#define empty 10

static uint8_t digit[4][4] ={{PA0,PA2,PA3,PA1},{PA4,PA6,PA7,PA5},{PD7,PD5,PD4,PD6},{PD3,PD1,PD0,PD2}};


static uint8_t nr_tab[11]={
	0b0000,  //0
	0b0001,  //1
	0b0010,  //2
	0b0011,  //3
	0b0100,  //4
	0b0101,  //5
	0b0110,  //6
	0b0111,  //7
	0b1000,  //8
	0b1001,  //9
	empty,
};

void disp_init(void);
void disp_set(uint8_t dig,uint8_t Nr);
void disp_2_val(uint8_t val1,uint8_t val2);
void disp_year(void);
void disp_time(void);
void disp_date(void);
void disp_done(void);
void disp_done_0(void);
void disp_empty(void);
void disp_one_digit(uint8_t nr);

#endif /* DISP_H_ */
