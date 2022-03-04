/*

 * ws2812.c
 *
 *  Created on: 20 gru 2017
 *      Author: Dv8
 */

#include "ws2812.h"
#include <avr/io.h>
#include <util/delay.h>

void ws2812_init(void){
	ws2812_port_init;
	uint8_t i;
	for(i=0;i<quantity;i++){
		red[i]=0;green[i]=0;blue[i]=0;
	}
}
void ws2812_set_single(uint8_t poz,uint8_t r,uint8_t g,uint8_t b){
	red[poz]=r;green[poz]=g;blue[poz]=b;
}
void ws2812_send(void){
	uint8_t i,a;
	for(i=0;i<quantity;i++){
		for(a=0;a<7;a++){
			if(((green[i]>>a)&1)==1){
				send_1;
			}else send_0;
		}
		for(a=0;a<7;a++){
			if(((red[i]>>a)&1)==1){
				send_1;
			}else send_0;
		}
		for(a=0;a<7;a++){
			if(((blue[i]>>a)&1)==1){
				send_1;
			}else send_0;
		}
	}
	reset;
}
