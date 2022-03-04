#include <avr/io.h>
#include <util/delay.h>
#include "i2c/i2c.h"
#include "ds1307/ds1307.h"
#include "tim/tim.h"
#include "menu/menu.h"
#include "buzzer/buzzer.h"
#include "disp/disp.h"
#include "eprom/eprom.h"
volatile uint16_t counter=0,duration;
volatile uint8_t sqw=0,i=0,czas;    //sterowanie wyjœciem diody sqw miga przy godzinie, przy dacie œwieci
volatile uint8_t menu_OFF = 1;
#define tim2_presc 40       // 25*40=1 sec
#define tim2_minHour tim2_presc*5
#define tim2_dayMounth (tim2_presc*2)+tim2_minHour

uint8_t day, mon,h,dT;
//uint16_t coun1 = 9;


ISR(TIMER0_COMP_vect){   //Licznik dla buzzera
	duration++;
	if(sqw)ds_checkSQW();
	else PORTC |= (1<<PC7);
}

ISR(TIMER1_COMPA_vect){  //wype³nienie 50% w³aœciwe sterowanie zadan¹ czêstotliwoœci¹ buzzerem
	PORTC ^= (1<<PC6);

}

ISR(TIMER2_COMP_vect){
	day = ds_readSingleReg(dayReg);
	day = (((day>>4)&0x0F)*10)+(day&0x0F);
	mon = ds_readSingleReg(mounthReg);
	mon = (((mon>>4)&0x0F)*10)+(mon&0x0F);
	uint16_t dsTime = ds_readTime();
	h=(dsTime>>8)&0xFF;
	dT = ds_readSingleReg(dTReg);
	if(menu_OFF&&counter<tim2_minHour){
		sqw=1;
		disp_time();
	}
	if(menu_OFF&&counter==tim2_minHour){
		sqw=0;
		disp_date();
	}
	counter++;
	if(counter==tim2_dayMounth)counter=0;
	//		PORTC ^= (1<<PC6);
}

void tone(uint32_t freq,uint16_t duration_ms){
	TIMSK |= (1<<OCIE1A);
	duration = 0;
	TCNT0 = 0;
	while(duration<=duration_ms){
		buzzer_setFreq(freq);
	}
	TIMSK &= ~(1<<OCIE1A);
	PORTC &= ~(1<<PC6);
}
void vader(void){
//	menu_OFF = 0;
	tone(LA3,Q);
//	menu_OFF = 0;
	_delay_ms(1+Q); //_delay_ms duration should always be 1 ms more than the note in order to separate them.
	tone(LA3,Q);
	_delay_ms(1+Q);
	tone(LA3,Q);
	_delay_ms(1+Q);
	tone(F3,E+S);
	_delay_ms(1+E+S);
	tone(C4,S);
	_delay_ms(1+S);

	tone(LA3,Q);
	_delay_ms(1+Q);
	tone(F3,E+S);
	_delay_ms(1+E+S);
	tone(C4,S);
	_delay_ms(1+S);
	tone(LA3,H);
	_delay_ms(1+H);

	tone(E4,Q);
	_delay_ms(1+Q);
	tone(E4,Q);
	_delay_ms(1+Q);
	tone(E4,Q);
	_delay_ms(1+Q);
	tone(F4,E+S);
	_delay_ms(1+E+S);
	tone(C4,S);
	_delay_ms(1+S);

	tone(Ab3,Q);
	_delay_ms(1+Q);
	tone(F3,E+S);
	_delay_ms(1+E+S);
	tone(C4,S);
	_delay_ms(1+S);
	tone(LA3,H);
	_delay_ms(1+H);

	tone(LA4,Q);
	_delay_ms(1+Q);
	tone(LA3,E+S);
	_delay_ms(1+E+S);
	tone(LA3,S);
	_delay_ms(1+S);
	tone(LA4,Q);
	_delay_ms(1+Q);
	tone(Ab4,E+S);
	_delay_ms(1+E+S);
	tone(G4,S);
	_delay_ms(1+S);

	tone(Gb4,S);
	_delay_ms(1+S);
	tone(E4,S);
	_delay_ms(1+S);
	tone(F4,E);
	_delay_ms(1+E);
	_delay_ms(1+E);//PAUSE
	tone(Bb3,E);
	_delay_ms(1+E);
	tone(Eb4,Q);
	_delay_ms(1+Q);
	tone(D4,E+S);
	_delay_ms(1+E+S);
	tone(Db4,S);
	_delay_ms(1+S);

	tone(C4,S);
	_delay_ms(1+S);
	tone(B3,S);
	_delay_ms(1+S);
	tone(C4,E);
	_delay_ms(1+E);
	_delay_ms(1+E);//PAUSE QUASI FINE RIGA
	tone(F3,E);
	_delay_ms(1+E);
	tone(Ab3,Q);
	_delay_ms(1+Q);
	tone(F3,E+S);
	_delay_ms(1+E+S);
	tone(LA3,S);
	_delay_ms(1+S);

	tone(C4,Q);
	_delay_ms(1+Q);
	tone(LA3,E+S);
	_delay_ms(1+E+S);
	tone(C4,S);
	_delay_ms(1+S);
	tone(E4,H);
	_delay_ms(1+H);

	tone(LA4,Q);
	_delay_ms(1+Q);
	tone(LA3,E+S);
	_delay_ms(1+E+S);
	tone(LA3,S);
	_delay_ms(1+S);
	tone(LA4,Q);
	_delay_ms(1+Q);
	tone(Ab4,E+S);
	_delay_ms(1+E+S);
	tone(G4,S);
	_delay_ms(1+S);

	tone(Gb4,S);
	_delay_ms(1+S);
	tone(E4,S);
	_delay_ms(1+S);
	tone(F4,E);
	_delay_ms(1+E);
	_delay_ms(1+E);//PAUSE
	tone(Bb3,E);
	_delay_ms(1+E);
	tone(Eb4,Q);
	_delay_ms(1+Q);
	tone(D4,E+S);
	_delay_ms(1+E+S);
	tone(Db4,S);
	_delay_ms(1+S);

	tone(C4,S);
	_delay_ms(1+S);
	tone(B3,S);
	_delay_ms(1+S);
	tone(C4,E);
	_delay_ms(1+E);
	_delay_ms(1+E);//PAUSE QUASI FINE RIGA
	tone(F3,E);
	_delay_ms(1+E);
	tone(Ab3,Q);
	_delay_ms(1+Q);
	tone(F3,E+S);
	_delay_ms(1+E+S);
	tone(C4,S);
	_delay_ms(1+S);

	tone(LA3,Q);
	_delay_ms(1+Q);
	tone(F3,E+S);
	_delay_ms(1+E+S);
	tone(C4,S);
	_delay_ms(1+S);
	tone(LA3,H);
	_delay_ms(1+H);

	_delay_ms(2*H);
}
int main(){

	ds_init();
	disp_init();
	tim_init();
	menu_init();
	DDRC |= (1<<PC6);
	PORTC |= (1<<PC6);
//	ds_setTime(25,10,23,1,10,5,21);
	czas = ((eprom_readFrom(eep_dayReg)>>7)&1);

	buzzer_init();

//	disp_2_val(12,34);

	while(1)
	{
		if(mon == 3 && day >= 24 && dT == 7 && h == 2 && czas == 0){
			uint8_t a = eprom_readFrom(eep_dayReg);
			a |= (1<<7);
			eprom_writeTo(eep_dayReg,a);
			h++;
			h = ((h/10)<<4)|(h%10);
			ds_setSingleReg(hourReg,h);
			czas = ((eprom_readFrom(eep_dayReg)>>7)&1);
		}
		if(mon == 10 && day >= 24 && dT == 7 && h == 3 && czas == 1){
			uint8_t a = eprom_readFrom(eep_dayReg);
			a &= ~(1<<7);
			eprom_writeTo(eep_dayReg,a);
			h--;
			h = ((h/10)<<4)|(h%10);
			ds_setSingleReg(hourReg,h);
			czas = ((eprom_readFrom(eep_dayReg)>>7)&1);
		}
		if(!button_1){  //wejœcie do menu
			menu_OFF=0; //wy³ przerwania
			menu_main();
			menu_OFF=1; //w³ przerwania
			counter=0;
		}
		while(!button_2){  //wyœwietl dzieñ tegodnia
			menu_OFF=0; //wy³ przerwania
			disp_one_digit(dT);
			menu_OFF=1; //w³ przerwania
			counter=0;
		}
		while(!button_3){  //wyœwietl rok
			menu_OFF=0; //wy³ przerwania
			disp_year();
			menu_OFF=1; //w³ przerwania
			counter=0;
		}
		while(!button_4){  //wyœwietl czas letni/zimowy
			menu_OFF=0; //wy³ przerwania
			disp_one_digit(czas);
			menu_OFF=1; //w³ przerwania
			counter=0;
		}
		if(!button_5){  //zmniana czasu letni/zimowy
			menu_OFF=0; //wy³ przerwania
			if(czas){
				uint8_t a = eprom_readFrom(eep_dayReg);
				a &= ~(1<<7);
				eprom_writeTo(eep_dayReg,a);
				disp_done_0();
				czas=0;
			}else{
				uint8_t a = eprom_readFrom(eep_dayReg);
				a |= (1<<7);
				eprom_writeTo(eep_dayReg,a);
				disp_done();
				czas=1;
			}
			menu_OFF=1; //w³ przerwania
			counter=0;
		}
		back_v nn = eeprom_checkAlarm();
//		disp_2_val(nn.dayreg_ep/100,nn.dayreg_ep%100);
//		disp_2_val(nn.Hour,nn.Min);
//		disp_2_val(nn.Hour_ep,nn.Min_ep);
//		disp_2_val(nn.Min,nn.Sec);
		if(nn.ret == 1){
			TIMSK &= ~(1<<OCIE2);
			vader();
			TIMSK |= (1<<OCIE2);
		}
	}
}

