/*
 * i2c.h
 *
 *  Created on: 24 paü 2017
 *      Author: Dv8
 */

#ifndef I2C_H_
#define I2C_H_

#include <avr/io.h>
#include <util/delay.h>


#if defined(_AVR_ATtiny44A_H_)
#define i2c_ddr DDRA  //bPn8aKDPGe4HkJ5p
#define i2c_pin PINA
#define i2c_port PORTA
#define scl_port PA4
#define sda_port PA5
#endif

#if defined(__AVR_ATmega328P__)
#define i2c_ddr DDRB  //bPn8aKDPGe4HkJ5p
#define i2c_pin PINB
#define i2c_port PORTB
#define scl_port PB0
#define sda_port PB1
#endif

#if defined(__AVR_ATmega32__)
#define i2c_ddr DDRC  //bPn8aKDPGe4HkJ5p
#define i2c_pin PINC
#define i2c_port PORTC
#define scl_port PC0
#define sda_port PC1
#endif

#if defined(__AVR_ATmega16__)
#define i2c_ddr DDRC  //bPn8aKDPGe4HkJ5p
#define i2c_pin PINC
#define i2c_port PORTC
#define scl_port PC0
#define sda_port PC1
#endif

#define scl_out i2c_ddr |= (1<<scl_port)
#define scl_one i2c_port |= (1<<scl_port)
#define scl_zero i2c_port &= ~(1<<scl_port)
#define scl_in i2c_ddr &= ~(1<<scl_port)
#define get_scl (i2c_pin & (1<<scl_port))
#define sda_out i2c_ddr |= (1<<sda_port)
#define sda_one i2c_port |= (1<<sda_port)
#define sda_zero i2c_port &= ~(1<<sda_port)
#define sda_in i2c_ddr &= ~(1<<sda_port)
#define get_sda !(i2c_pin & (1<<sda_port))

#define delay1 _delay_us(2)
#define delay2 _delay_us(4)



	void i2c_init(void);
	void i2c_start(void);
	void i2c_stop(void);
	void i2c_requestFrom(uint8_t addr);
	void i2c_sendTo(uint8_t addr);
	uint8_t i2c_send_byte(uint8_t data);
	void i2c_send(uint8_t slaveAddr,uint8_t data,uint8_t stop);
	uint8_t i2c_read(uint8_t slaveAddr,uint8_t stop);
	uint8_t i2c_read_byte(uint8_t stop);

#endif /* I2C_H_ */
