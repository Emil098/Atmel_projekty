/*

 * hc05.c
 *
 *  Created on: 19 gru 2017
 *      Author: Dv8
 */

#include "hc05.h"
#include <avr/io.h>
#include <util/delay.h>
#include "../uart/uart.h"

void HC05_begin(){
	uart_init();
}
uint16_t HC05_recive(){
	uint8_t done=0;
	while(done==0){
		int recived = uart_receive();
		while(recived==-1)recived = uart_receive();
		if(state){
			data[counter]=recived;
			counter++;
			if(counter==3){
				counter=0;
				state=0;
				done=1;
			}
		}
		if(recived==3){
			state=1;
		}
	}
	return (data[2]*100)+(data[1]*10)+data[0];
}
void HC05_send(uint16_t value){
	int current;
	uint8_t i;
	uart_send_nr(3);
	for(i=0;i<3;i++){
		_delay_ms(6);
		current=value/(pow(10,i));
		uart_send_nr(current%10);
	}
	_delay_ms(16);
}

