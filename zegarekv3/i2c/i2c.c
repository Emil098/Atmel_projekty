#include "i2c.h"
#include <avr/io.h>
#include <util/delay.h>

void i2c_init(void){
	scl_out;
	sda_out;
	scl_one;
	sda_one;
}
void i2c_start(void){
	scl_out;
	sda_out;
	delay2;
	scl_one;
	sda_one;
	delay1;
	sda_zero;
	delay1;
	scl_zero;
}
void i2c_stop(void){
	delay1;
	sda_out;
	sda_zero;
	delay1;
	scl_one;
	delay1;
	sda_one;
}
void i2c_requestFrom(uint8_t addr){
	i2c_send_byte((addr<<1)|1);
}
void i2c_sendTo(uint8_t addr){
	i2c_send_byte((addr<<1)|0);
}
uint8_t i2c_send_byte(uint8_t data){
	uint8_t i;
	sda_out;
	delay1;
	for(i=0;i<8;i++){
			if((data>>(7-i)) & 1)sda_one;
			else sda_zero;
			delay1;
			scl_one;
			delay2;
			scl_zero;
			delay1;
	}
	sda_in;
	delay1;
	scl_one;
	if(get_sda & 1)i=1;
	else i=0;
	delay2;
	scl_zero;
	if(i)return 1;
	else return 0;
}
void i2c_send(uint8_t slaveAddr,uint8_t data,uint8_t stop){
	//	cli();
	//uint8_t i;
	i2c_start();
	i2c_send_byte((slaveAddr<<1)|0);
	i2c_send_byte(data);
	//if(!i)led2_off;
	if(stop)i2c_stop();
//	sei();
}
uint8_t i2c_read(uint8_t slaveAddr,uint8_t stop){
	i2c_start();
	uint8_t i,c,val=0;
	i = i2c_send_byte((slaveAddr<<1)|1);
	sda_in;
	sda_one;
	for(i=0;i<8;i++){
		delay2;
		scl_one;
		delay1;
		c = get_sda;
		if(c==1){
			//led2_off;
			val |= (1<<(7-i));
		}
		delay1;
		scl_zero;
	}
	delay1;
	sda_out;
	sda_zero;
	delay1;
	scl_one;
	delay2;
	scl_zero;
	if(stop)i2c_stop();
	val = ~val;
	return val;
}
uint8_t i2c_read_byte(uint8_t stop){
	uint8_t i,c,val=0;
	sda_in;
	sda_one;
	for(i=0;i<8;i++){
		delay2;
		scl_one;
		delay1;
		c = get_sda;
		if(c==1){
			//led2_off;
			val |= (1<<(7-i));
		}
		delay1;
		scl_zero;
	}
	delay1;
	sda_out;
	if(stop)sda_zero;
	else sda_one;
	delay1;
	scl_one;
	delay2;
	scl_zero;
	delay1;
	if(stop)sda_zero;
	delay1;
	scl_one;
	delay2;
	scl_zero;
	if(stop)i2c_stop();
	val = ~val;
	return val;
}
