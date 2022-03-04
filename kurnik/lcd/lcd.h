#ifndef LCD_H_
#define LCD_H_


#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "types.h"

//nag³ówki funkcji:
#define D4 7
#define D5 6
#define D6 5
#define D7 4
#define EN 8
#define RS 9

#define D4_pin PD3
#define D5_pin PD2
#define D6_pin PD1
#define D7_pin PD0
#define EN_pin PD4
#define RS_pin PD6
#define RW_pin PD5

#define HIGH 1
#define LOW 0

void setup(void);
void loop(void);
void lcd_send();
void lcd_time(uint8_t h,uint8_t m,uint8_t mon,uint8_t day1,uint8_t year1);
void lcd_wschod_zachod(uint8_t wsH,uint8_t wsM,uint8_t zaH,uint8_t zaM);
void lcd_putstring(char* StringPtr);
void lcd_send_nr(int nr);
void ustaw_lcd(int wiersz,int kolumna);
void zrob_znak(uint8_t addr,uint8_t tworzony_znak[9]);
void lcd_pisz(int bajt);
void lcd_clear();
void lcd_Init_blink();
void lcd_Init_no_blink();
void digitalWrite(uint8_t pin,uint8_t stan);
void setC(uint8_t c);

//sta³e dla SetPolishChars
#define POL_a "\x08"
#define POL_c "\x09"
#define POL_e "\x0A"
#define POL_l "\x0B"
#define POL_n "\x0C"
#define POL_o "\x0D"
#define POL_s "\x0E"
#define POL_z "\x0F"

#endif /*LCD_H_*/
