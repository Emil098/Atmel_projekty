/*
 * menu.c
 *
 *  Created on: 2 mar 2020
 *      Author: Emil
 */

#include <avr/io.h>
#include <util/delay.h>
#include "menu.h"
#include "D:\projekty eclipse\kurnik_1\lcd\lcd.h"
#include "D:\projekty eclipse\kurnik_1\adc\adc.h"
#include "D:\projekty eclipse\kurnik_1\ds1307\ds1307.h"


void button_init(){
	EXT_BUTTON_REG &= ~((1<<EXT_BUTTON_1)|(1<<EXT_BUTTON_2)|(1<<EXT_BUTTON_3));
	INT_BUTTON_REG &= ~((1<<INT_BUTTON_1)|(1<<INT_BUTTON_2)|(1<<INT_BUTTON_3)|(1<<INT_BUTTON_4));
}

void check_adc(){
	uint8_t hour2;
	hour2 = ds_readSingleReg(hourReg);
	hour2 = (((hour2>>4)&0x0F)*10)+(hour2&0x0F);
	uint16_t adc2 = adc_getdata(),adc_res2 = 750;
	if(hour2>=7&&hour2<=18){
//		wyl_pwm_i_ustaw_stan(0);
		if(adc2>(adc_res2 + 25)){
			wyl_pwm_i_ustaw_stan(1);
			PWM_LED_OFF;
		}
		if(adc2<(adc_res2 - 25)){
			wyl_pwm_i_ustaw_stan(0);
			PWM_LED_ON;
		}
		if(adc2>(adc_res2-25) && adc2<(adc_res2+25)){
		}
	}
}

uint8_t check_buttons(){
	uint8_t ret=0;
	if(INT_BUTTON_1_ST){
		ret=1;
		LED_LCD_ON;// tryb menu
		_delay_ms(1000);
		if(INT_BUTTON_1_ST){
			MENU_LED_ON;
			uint8_t b1;
			b1 = service_menu();
			while(b1==1){
				b1=service_menu();
			}
		}
	}
	if(EXT_BUTTON_1_ST){
		ret=1;
		LED_LCD_ON;
		open_door();
	}
	if(EXT_BUTTON_2_ST){
		ret=1;
		LED_LCD_ON;
		close_door();
	}
	if(EXT_BUTTON_3_ST){
		ret=1;
		LED_LCD_ON;
		if(((PORTB>>PB1)&1)==1){
			DOOR_LED_ON;
		}else{
			DOOR_LED_OFF;
		}
	}
	return ret;
}
void close_door(){
	if(((PORTB>>PB1)&1)==1){
		PHASE_0;                 // zamykanie
		SLEEP_1;
		uint8_t i;
		lcd_clear();
		lcd_putstring("zamykanie: ");
		for(i=0;i<10;i++){
			ustaw_lcd(0,12);
			lcd_send_nr(i*10);
			lcd_putstring("%");
			_delay_ms(actuator_delay/10);
		}
		SLEEP_0;
	}
}
void open_door(){
	if(((PORTB>>PB1)&1)==1){
		PHASE_1;                 // otwieranie
		SLEEP_1;
		uint8_t i;
		lcd_clear();
		lcd_putstring("otwieranie: ");
		for(i=0;i<10;i++){
			ustaw_lcd(0,13);
			lcd_send_nr(i*10);
			lcd_putstring("%");
			_delay_ms(actuator_delay/10);
		}
		SLEEP_0;
	}
}
uint8_t set_date(){
	uint8_t back=0;
	uint8_t t_min=0,t_hour=0,t_day=1,t_dayOfWeek=1,t_mounth=1,t_year=20;

	t_min = ds_readSingleReg(minReg);
	t_min = (((t_min>>4)&0x0F)*10)+(t_min&0x0F);
	t_hour = ds_readSingleReg(hourReg);
	t_hour = (((t_hour>>4)&0x0F)*10)+(t_hour&0x0F);
	t_day = ds_readSingleReg(dayReg);
	t_day = (((t_day>>4)&0x0F)*10)+(t_day&0x0F);
	t_mounth = ds_readSingleReg(mounthReg);
	t_mounth = (((t_mounth>>4)&0x0F)*10)+(t_mounth&0x0F);
	t_year = ds_readSingleReg(yearReg);
	t_year = (((t_year>>4)&0x0F)*10)+(t_year&0x0F);


	MENU_LED_BLINK;
	_delay_ms(400);
	lcd_clear();
	lcd_putstring("godzina:   00:00");
	while(!INT_BUTTON_1_ST&&back==0){
		if(INT_BUTTON_4_ST)back=1;
		ustaw_lcd(0,15);
		if(t_min<10)lcd_putstring("0");
		lcd_send_nr(t_min);
		ustaw_lcd(0,15);
		_delay_ms(200);
		if(INT_BUTTON_2_ST)t_min++;
		if(INT_BUTTON_3_ST){
			if(t_min==0)t_min=60;
			if(t_min>0)t_min--;
		}
		if(t_min==60)t_min=0;
	}
	MENU_LED_BLINK;
	_delay_ms(400);
	while(!INT_BUTTON_1_ST&&back==0){
		if(INT_BUTTON_4_ST)back=1;
		ustaw_lcd(0,12);
		if(t_hour>=10){
			ustaw_lcd(0,11);
			lcd_putstring(" ");
		}else{
			ustaw_lcd(0,10);
			lcd_putstring("  ");
		}
		lcd_send_nr(t_hour);
		ustaw_lcd(0,11);
		lcd_putstring(" ");
		_delay_ms(200);
		if(INT_BUTTON_2_ST)t_hour++;
		if(INT_BUTTON_3_ST){
			if(t_hour==0)t_hour=24;
			if(t_hour>0)t_hour--;
		}
		if(t_hour==24)t_hour=0;
	}
	MENU_LED_BLINK;
	_delay_ms(400);
	ustaw_lcd(1,0);
	if(back==0)lcd_putstring("data: 2020-01-01");  //tu
	while(!INT_BUTTON_1_ST&&back==0){
		if(INT_BUTTON_4_ST)back=1;
		ustaw_lcd(1,15);
		if(t_day<10)lcd_putstring("0");
		lcd_send_nr(t_day);
		ustaw_lcd(1,15);
		_delay_ms(200);
		if(INT_BUTTON_2_ST)t_day++;
		if(INT_BUTTON_3_ST){
			if(t_day==1)t_day=32;
			if(t_day>1)t_day--;
		}
		if(t_day==32)t_day=1;
	}
	MENU_LED_BLINK;
	_delay_ms(400);
	while(!INT_BUTTON_1_ST&&back==0){
		if(INT_BUTTON_4_ST)back=1;
		ustaw_lcd(1,12);
		if(t_mounth>=10){
			ustaw_lcd(1,11);
			lcd_putstring("-");
		}else{
			ustaw_lcd(1,10);
			lcd_putstring("-0");
		}
		lcd_send_nr(t_mounth);
		ustaw_lcd(1,11);
		lcd_putstring("-");
		_delay_ms(200);
		if(INT_BUTTON_2_ST)t_mounth++;
		if(INT_BUTTON_3_ST){
			if(t_mounth==1)t_mounth=13;
			if(t_mounth>1)t_mounth--;
		}
		if(t_mounth==13)t_mounth=1;
	}
	MENU_LED_BLINK;
	_delay_ms(400);
	while(!INT_BUTTON_1_ST&&back==0){
		if(INT_BUTTON_4_ST)back=1;
		ustaw_lcd(1,9);
		if(t_year<10)lcd_putstring("0");
		lcd_send_nr(t_year);
		ustaw_lcd(1,9);
		_delay_ms(200);
		if(INT_BUTTON_2_ST)t_year++;
		if(INT_BUTTON_3_ST){
			if(t_year==1)t_year=32;
			if(t_year>1)t_year--;
		}
		if(t_year==100)t_year=1;
	}
	MENU_LED_BLINK;
	_delay_ms(400);
	lcd_clear();
	if(back==0)lcd_putstring("dzien tygodnia");  //tu
	while(!INT_BUTTON_1_ST&&back==0){
		if(INT_BUTTON_4_ST)back=1;
		ustaw_lcd(1,0);
		lcd_send_nr(t_dayOfWeek);
		ustaw_lcd(1,0);
		_delay_ms(200);
		if(INT_BUTTON_2_ST)t_dayOfWeek++;
		if(INT_BUTTON_3_ST){
			if(t_dayOfWeek==1)t_dayOfWeek=8;
			if(t_dayOfWeek>1)t_dayOfWeek--;
		}
		if(t_dayOfWeek==8)t_dayOfWeek=1;
	}
	if(back==0)MENU_LED_BLINK; //tu
	if(back==0)ds_setTime(0,t_min,t_hour,t_dayOfWeek,t_day,t_mounth,t_year);  //tu
	lcd_clear();
	if(back==0)lcd_putstring("zegar ustawiony");  //tu
	_delay_ms(1000);
	return back;
}
void show_adc(){
	while(!INT_BUTTON_4_ST){
		MENU_LED_BLINK;
		uint16_t adc = adc_getdata();
		lcd_clear();
		lcd_send_nr(adc);
		_delay_ms(200);
	}
}
uint8_t service_menu(){
	uint8_t b;
	lcd_clear();
	lcd_putstring("wchodze do menu");
	_delay_ms(2000);
	MENU_LED_BLINK;
	lcd_clear();
	uint8_t index_menu = 1,zmiana=1;
	while(!INT_BUTTON_1_ST){
		MENU_LED_BLINK;
		if(index_menu==1&&zmiana){
			lcd_putstring("ustaw czas");zmiana=0;
		}
		if(index_menu==2&&zmiana){
			lcd_putstring("zobacz adc");zmiana=0;
		}
		if(INT_BUTTON_2_ST){
			index_menu++;
			zmiana=1;
			lcd_clear();
			MENU_LED_BLINK;
		}
		//		if(INT_BUTTON_3_ST){
		//			index_menu--;
		//			zmiana=1;
		//			lcd_clear();
		//			MENU_LED_BLINK;
		//		}
		if(index_menu==3)index_menu=1;
		if(index_menu==0)index_menu=2;
		if(INT_BUTTON_4_ST){
			index_menu=4;
			break;
		}
	}
	if(index_menu==1){
		setC(1);
		b=set_date();
		setC(0);
	}
	if(index_menu==2)show_adc();
	MENU_LED_OFF;
	return b;

}
