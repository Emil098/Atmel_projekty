/*
 * lcd.c
 *
 * Created: 2015-03-11 11:34:50
 *  Author: admin
 */ 




//#include "types.h"
#include "lcd.h"
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
//#include "rtc.h"


#define wyslij lcd_pisz(0b00001100)
#define wyslij1 lcd_pisz(0b00001111)
int piny[4] = {D4,D5,D6,D7};
uint8_t stopien[9] = {0x02,0x05,0x02,0x00,0x00,0x00,0x00,0x00};
uint8_t cursor=0;


void setup(void) {
	lcd_Init_blink();
	_delay_ms(500);
//	lcd_putstring("start");
//	lcd_putstring("dziala0");
	lcd_putstring("start systemu");
//	lcd_send_nr(123);
//	lcd_pisz(0x7E);
	_delay_ms(1000);
//	tim_set();
//	lcd_clear();
}
void setC(uint8_t c){
	cursor=c;
}
void lcd_time(uint8_t h,uint8_t m,uint8_t mon,uint8_t day1,uint8_t year1){

	ustaw_lcd(0,0);
	lcd_send_nr(h);
	lcd_putstring(":");
	if(m<10)lcd_putstring("0");
	lcd_send_nr(m);
	ustaw_lcd(1,0);
	lcd_send_nr(day1);
	lcd_putstring("-");
	if(day1<10)lcd_putstring("0");
	lcd_send_nr(mon);
	lcd_putstring("-20");
	lcd_send_nr(year1);
}
void lcd_wschod_zachod(uint8_t wsH,uint8_t wsM,uint8_t zaH,uint8_t zaM){
	ustaw_lcd(0,0);
	lcd_putstring("wschod ");
	lcd_send_nr(wsH);
	lcd_putstring(":");
	if(wsM<10)lcd_putstring("0");
	lcd_send_nr(wsM);
	ustaw_lcd(1,0);
	lcd_putstring("zachod ");
	lcd_send_nr(zaH);
	lcd_putstring(":");
	if(zaM<10)lcd_putstring("0");
	lcd_send_nr(zaM);

}
void lcd_putstring(char* StringPtr){
	digitalWrite(RS,HIGH);
	while(*StringPtr != 0){
		lcd_pisz(*StringPtr);
		StringPtr++;
	}
	lcd_send();
	//  tim_set();
}
void lcd_send(){
	digitalWrite(RS,LOW);
	_delay_ms(10);
	if(cursor)wyslij1;
	if(!cursor)wyslij;
	digitalWrite(RS,HIGH);
}
void lcd_send_nr(int nr){
	char str[16];
	itoa(nr, str, 10);
	lcd_putstring(str);
}
void loop(void){
	ustaw_lcd(1,0);
	digitalWrite(RS,HIGH);
	_delay_ms(10);
	lcd_pisz('E');
	lcd_pisz('m');
	lcd_pisz('i');
	lcd_pisz('l');
	lcd_pisz(0b11011111);
	_delay_ms(10);
	digitalWrite(RS,LOW);

	//digitalWrite(RS,LOW);
	_delay_ms(10);
	digitalWrite(RS,LOW);
	_delay_ms(10);
	wyslij;  //wysy³anie
	//lcd_pisz(0x30);
	_delay_ms(1000);
	lcd_clear();
	// }
}



void ustaw_lcd(int wiersz,int kolumna){
	int poz = 0;
	if(wiersz == 0){
		poz = (poz + kolumna)|0x81;
	}
	if(wiersz == 1){
		poz = ((poz)+kolumna)|0xC1;
	}
	digitalWrite(RS,LOW);
	_delay_ms(10);
	lcd_pisz(poz);
	_delay_ms(10);
	lcd_pisz(0b00010011);  //kursor
	digitalWrite(RS,HIGH);
	//  tim_set();
}
void zrob_znak(uint8_t addr,uint8_t tworzony_znak[9]){
	digitalWrite(RS,HIGH);

}
void digitalWrite(uint8_t pin,uint8_t stan){
	if(stan == HIGH){
		switch(pin){
		case D4:
			PORTD |= (1<<D4_pin);
			break;
		case D5:
			PORTD |= (1<<D5_pin);
			break;
		case D6:
			PORTD |= (1<<D6_pin);
			break;
		case D7:
			PORTD |= (1<<D7_pin);
			break;
		case EN:
			PORTD |= (1<<EN_pin);
			break;
		case RS:
			PORTD |= (1<<RS_pin);
			break;
		}
	}
	if(stan == LOW){
		switch(pin){
		case D4:
			PORTD &= ~(1<<D4_pin);
			break;
		case D5:
			PORTD &= ~(1<<D5_pin);
			break;
		case D6:
			PORTD &= ~(1<<D6_pin);
			break;
		case D7:
			PORTD &= ~(1<<D7_pin);
			break;
		case EN:
			PORTD &= ~(1<<EN_pin);
			break;
		case RS:
			PORTD &= ~(1<<RS_pin);
			break;
		}
	}
}
void lcd_pisz(int Bajt){
	int c[8];
	for(int i = 0; i < 8; i++){
		c[i] = (Bajt >> i) & 0x1;
	}
	digitalWrite(EN,HIGH);
	digitalWrite(D7,c[7]);
	digitalWrite(D6,c[6]);
	digitalWrite(D5,c[5]);
	digitalWrite(D4,c[4]);
	digitalWrite(EN,LOW);
	for(int i=0;i<2;i++){
	}
	digitalWrite(EN,HIGH);
	digitalWrite(D7,c[3]);
	digitalWrite(D6,c[2]);
	digitalWrite(D5,c[1]);
	digitalWrite(D4,c[0]);
	digitalWrite(EN,LOW);
	_delay_ms(1);
}
void lcd_clear(){
	digitalWrite(RS,LOW);
	lcd_pisz(0b00000001);
	digitalWrite(RS,HIGH);
	_delay_ms(4);
	//  tim_set();
}
void lcd_Init_no_blink(){
	DDRD |= (1<<D4_pin)|(1<<D5_pin)|(1<<D6_pin)|(1<<D7_pin)|(1<<EN_pin)|(1<<RS_pin)|(1<<RW_pin);
	PORTD &= ~(1<<RW_pin);
	digitalWrite(RS,LOW);
	digitalWrite(EN,LOW);
	for(int i=0;i<4;i++){
		digitalWrite(piny[i],LOW);
	}
	_delay_ms(45);
	lcd_pisz(0b00101000);
	digitalWrite(RS,HIGH);
	digitalWrite(RS,LOW);
	lcd_pisz(0b00000110);
	digitalWrite(RS,HIGH);
	digitalWrite(RS,LOW);
	lcd_pisz(0b00001101);
	digitalWrite(RS,HIGH);
	digitalWrite(RS,LOW);
	lcd_clear();
//	wyslij;
}
void lcd_Init_blink(){
	DDRD |= (1<<D4_pin)|(1<<D5_pin)|(1<<D6_pin)|(1<<D7_pin)|(1<<EN_pin)|(1<<RS_pin)|(1<<RW_pin);
	PORTD &= ~(1<<RW_pin);
	digitalWrite(RS,LOW);
	digitalWrite(EN,LOW);
	for(int i=0;i<4;i++){
		digitalWrite(piny[i],LOW);
	}
	_delay_ms(45);
	lcd_pisz(0b00101000);
	digitalWrite(RS,HIGH);
	digitalWrite(RS,LOW);
	lcd_pisz(0x10);
	digitalWrite(RS,HIGH);
	digitalWrite(RS,LOW);
//	lcd_pisz(0b00001111);
	lcd_pisz(0x0F);
	digitalWrite(RS,HIGH);
	digitalWrite(RS,LOW);
//	lcd_pisz(0b00010000);
	lcd_pisz(0x06);
	digitalWrite(RS,HIGH);
	digitalWrite(RS,LOW);
	lcd_clear();
//	wyslij;
}
