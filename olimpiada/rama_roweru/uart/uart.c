/*

 * uart.c
 *
 *  Created on: 19 gru 2017
 *      Author: Dv8
 */
#include "uart.h"
#include <avr/io.h>
#include <util/delay.h>

void uart_send_nr(int nr){
	char str[16];
	itoa(nr, str, 10);
	uart_putstring(str);
	uart_send('\n');
}
void uart_init(void){

 UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
 UBRR0L = (uint8_t)(BAUD_PRESCALLER);  // 9600 bps
 UCSR0B = (1<<RXEN0)|(1<<TXEN0);  //w³¹czenie linii rx i txb
 UCSR0C = (3<<UCSZ00);  //tryb asynchroniczny s³owo 8 bitowe
}

unsigned char uart_receive(void){

 while(!(UCSR0A & (1<<RXC0)));  //nowe dane odebrane
 return UDR0;

}

void uart_send( unsigned char data){

 while(!(UCSR0A & (1<<UDRE0)));  //czeka a¿ bufor odbieranych danych bêdzie pusty
 UDR0 = data;

}

void uart_putstring(char* StringPtr){

while(*StringPtr != 0){
 uart_send(*StringPtr);
 StringPtr++;
}
}


