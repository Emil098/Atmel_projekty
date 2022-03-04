/*
 * ws2812.h
 *
 *  Created on: 20 gru 2017
 *      Author: Dv8
 */

#ifndef WS2812_WS2812_H_
#define WS2812_WS2812_H_
#include <avr/io.h>
#include <util/delay.h>

/* ustawienia portu */
#define ws2812_port PORTD
#define ws2812_ddr DDRD
#define ws2812_pin PD4
#define ws2812_port_init ws2812_ddr |= (1<<ws2812_pin)
#define set_1 ws2812_port |= (1<<ws2812_pin)
#define set_0 ws2812_port &= ~(1<<ws2812_pin)

// czêstotliwoœæ kwarcu procesora to 16MHz co dajê nam 1 cykl zegarowy o czasie 62 ns
// T0H to 6 cykli
#define T0H asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop")
// T1H to 13 cykli
#define T1H asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop")
// T0L to 14 cykli
#define T0L asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop")
// T1L to 7 cykli
#define T1L asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");asm volatile ("nop")

#define send_0 set_1;T0H;set_0;T0L;set_1  // wysy³a logiczne "0" do szeregu diod
#define send_1 set_1;T1H;set_0;T1L;set_1  // wysy³a logiczne "1" do szeregu diod
#define reset set_0;_delay_us(55);set_1

#define quantity 48 // iloœæ diod
uint8_t red[quantity],green[quantity],blue[quantity]; // tablice globalne przechowuj¹ce dane które bêd¹ wys³ane do diod

void ws2812_init(void);  // ustawinie portu jako wyjœcie i wyzerowanie tablic
void ws2812_set_single(uint8_t poz,uint8_t r,uint8_t g,uint8_t b);
void ws2812_send(void);

#endif /* WS2812_WS2812_H_ */
