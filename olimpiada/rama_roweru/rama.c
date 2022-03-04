/*

 * main.c
 *
 *  Created on: 18 gru 2017
 *      Author: Dv8
 */
#include <avr/io.h>
#include <util/delay.h>
#include "hc05/hc05.h"
#include "i2c/i2c.h"
#include "lsm303/lsm303.h"
#include "uart/uart.h"
int main(){
	lsm303_init();
	HC05_begin();
	while(1){
		uint16_t heading = lsm303_heading();
		HC05_send(heading);
	}
}
