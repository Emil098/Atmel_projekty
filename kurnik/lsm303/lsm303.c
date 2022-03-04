/*
 * lsm303.c
 *
 *  Created on: 3 mar 2020
 *      Author: Emil
 */

#include <avr/io.h>
#include <util/delay.h>
#include "lsm303.h"


void lm_init(){
	i2c_start();
	i2c_send_byte(0b10010000);
	i2c_send_byte(LM75_REG_CONF);
	i2c_send_byte(0);
	i2c_stop();

}
int lm_read(){
	uint8_t data[2];
	i2c_start();
	i2c_send_byte(0b10010000);
	i2c_send_byte(0);
	i2c_start();
	i2c_send_byte(0b10010001);
	data[0] = i2c_read_byte(0);
	data[1] = i2c_read_byte(0);
	i2c_stop();
	uint16_t temp = ((data[0]<<8)|data[1]);
	int temp1 = ((temp>>8)&0b1111111);
	if(((temp>>15)&1)==1){
		temp = !temp;
		temp1 = ((temp>>8)&0b1111111);
		temp1 *= (-1);
	}


	return temp1;
}

