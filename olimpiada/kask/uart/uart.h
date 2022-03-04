/*
 * uart.h
 *
 *  Created on: 19 gru 2017
 *      Author: Dv8
 */

#ifndef UART_UART_H_
#define UART_UART_H_
#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 16000000UL
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

void USART_init(void);
void USART_send_nr(int nr);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);

#endif /* UART_UART_H_ */
