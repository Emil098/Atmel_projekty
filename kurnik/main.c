
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "i2c/i2c.h"
#include "eprom/eprom.h"
#include "adc/adc.h"
#include "ds1307/ds1307.h"
#include "calc/calc.h"
#include "lcd/lcd.h"
#include "menu/menu.h"
#include "lsm303/lsm303.h"

#define THIS_POSITION_LAT  51.474606 //51.245800
#define THIS_POSITION_LON  22.430861 //22.635920

//faza pa2  sleep pa3
#define motor_init DDRA |= (1<<PA3)|(1<<PA2);DDRC |= (1<<PC6)|(1<<PC7);DDRB |= (1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB7)  // sterownik dla silownika i oswietlenia
#define SLEEP_1 PORTA |= (1<<PA2);PORTC |= (1<<PC7)
#define SLEEP_0 PORTA &= ~(1<<PA2);PORTC &= ~(1<<PC7)
#define PHASE_1 PORTA |= (1<<PA3)
#define PHASE_0 PORTA &= ~(1<<PA3)
#define ENABLE_PWM TCCR2 |= (1<<COM21)
#define DISABLE_PWM TCCR2 &= ~(1<<COM21)
#define adc_res 750

#define LED_EN_1 PORTC |= (1<<PC6)
#define LED_EN_0 PORTC &= ~(1<<PC6)


#define LED_LCD_OFF PORTB &= ~(1<<PB7)
#define LED_LCD_ON PORTB |= (1<<PB7)
#define MENU_LED_OFF PORTB |= (1<<PB0)
#define MENU_LED_ON PORTB &= ~(1<<PB0)
#define DOOR_LED_OFF PORTB |= (1<<PB1)
#define DOOR_LED_ON PORTB &= ~(1<<PB1)
#define PWM_LED_OFF PORTB |= (1<<PB2)
#define PWM_LED_ON PORTB &= ~(1<<PB2)
#define actuator_delay 45000
//#define MENU_LED_BLINK MENU_LED_ON;_delay_ms(500);MENU_LED_OFF;_delay_ms(500)

uint16_t amin,amax;
uint16_t adata;

int lm_temp,lm_temp_max,lm_temp_min;

//uint16_t adc_res;
uint8_t service_mode=0,sec,hour,min,day,mou,year,disp_on=0,i=0;





void wyl_pwm_i_ustaw_stan(uint8_t stan){
	//	TIMSK &= ~(1<<OCIE2);
	DISABLE_PWM;
	if(stan){
		LED_EN_1;
		PORTD |= (1<<PD7);
	}else{
		LED_EN_0;
		PORTD &= ~(1<<PD7);
	}
}
void wl_pwm(uint8_t wart){
	//	TIMSK |= (1<<OCIE2);
	ENABLE_PWM;
	LED_EN_1;
	OCR2 = wart;
}
void set_hour_12(void){
	uint8_t dt = ds_readSingleReg(dTReg);
	//	dt = (((>>4)&0x0F)*10)+(min&0x0F);
	uint8_t day = ds_readSingleReg(dayReg);
	day = (((day>>4)&0x0F)*10)+(day&0x0F);
	uint8_t mounth = ds_readSingleReg(mounthReg);
	mounth = (((mounth>>4)&0x0F)*10)+(mounth&0x0F);
	uint8_t year = ds_readSingleReg(yearReg);
	year = (((year>>4)&0x0F)*10)+(year&0x0F);
	ds_setTime(0,0,12,dt,day,mounth,year);
}
void port_init(void){
	//	DDRA &= ~(1<<PA3);  //�wiat�o bez pwm
	//	PORTA &= ~(1<PA0);  //na pocz�tku zgaszone
	DDRB = 0xFF;  // pwm
	//	DDRB &= ~((1<<PB5)|(1<<PB0)|(1<<PB6)|(1<<PB1)|(1<<PB4));  //przyciski STARE !!!
	PORTB = (1<<PB5)|(1<<PB6)|(1<<PB0)|(1<<PB1)|(1<<PB4);  //pullup
	DDRD |= (1<<PD6);  //PD6 DIODA OD MENU
	PORTB |= (1<<PB0)|(1<<PB1)|(1<<PB2); // DIODY OD SYGNALIZACJI
	PORTD |= (1<<PD6);
}
void eeprom_init(void){
	uint8_t adl,adh;
	adl=eprom_readFrom(0);
	adh=eprom_readFrom(1);
//	adc_res = (adh<<8)|adl;
}
int main(void){

	adc_init();  //adc1 na pa1
	port_init();
	button_init();
	motor_init;
	setup();
	eeprom_init();  //do zapisu skrajnej warto�ci ADC
	DDRD |= (1<<PD7);// PIN od pwm
	TCCR2 = (1<<WGM20)|(1<<COM21)|(1<<CS21);
	OCR2 = 0;
	TCNT2 = 0;
	TIMSK |= (1<<OCIE2);
	//	ds_setTime(0,37,13,1,2,3,20);
	min = ds_readSingleReg(minReg);
	min = (((min>>4)&0x0F)*10)+(min&0x0F);
	hour = ds_readSingleReg(hourReg);
	hour = (((hour>>4)&0x0F)*10)+(hour&0x0F);
	_delay_ms(1000);
	lm_temp = lm_read();
	lm_temp_min = lm_temp;
	lm_temp_max = lm_temp;
	//	hour=5;min=5;
	//SLEEP_1;
	//		ds_setTime(50,9,6,2,3,3,20);
	while(1){



		uint16_t adc = adc_getdata();
		uint16_t val=0xFFF,val1;

		sec = ds_readSingleReg(secReg);
		sec = (((sec>>4)&0x0F)*10)+(sec&0x0F);
		min = ds_readSingleReg(minReg);
		min = (((min>>4)&0x0F)*10)+(min&0x0F);
		hour = ds_readSingleReg(hourReg);
		hour = (((hour>>4)&0x0F)*10)+(hour&0x0F);
		day = ds_readSingleReg(dayReg);
		day = (((day>>4)&0x0F)*10)+(day&0x0F);
		mou = ds_readSingleReg(mounthReg);
		mou = (((mou>>4)&0x0F)*10)+(mou&0x0F);
		year = ds_readSingleReg(yearReg);
		year = (((year>>4)&0x0F)*10)+(year&0x0F);

		disp_on = check_buttons();
		lm_temp = lm_read();

		if(disp_on){
			i++;
			disp_on=0;
		}
		if(i>0&&i<4)i++;
		if(i==4){
			i=0;
			LED_LCD_OFF;
		}
		if(lm_temp>lm_temp_max)lm_temp_max=lm_temp;
		if(lm_temp<lm_temp_min)lm_temp_min=lm_temp;


		lcd_clear();
		lcd_time(hour,min,mou,day,year);
		_delay_ms(4000);
		disp_on = check_buttons();
		if(disp_on)i++;
		check_adc();
		lcd_clear();
		lcd_wschod_zachod(cSunriseH,cSunriseM,cSunsetH,cSunsetM);
		_delay_ms(4000);
		disp_on = check_buttons();
		if(disp_on)i++;
		check_adc();
		lcd_clear();
		lcd_putstring("temp");
		ustaw_lcd(1,0);
		lcd_send_nr(lm_temp);
		lcd_pisz(0xB2);
		lcd_putstring("C");
		ustaw_lcd(0,8);
		lcd_putstring("max-");
		lcd_send_nr(lm_temp_max);
		lcd_pisz(0xB2);
		lcd_putstring("C");
		ustaw_lcd(1,8);
		lcd_putstring("min-");
		lcd_send_nr(lm_temp_min);
		lcd_pisz(0xB2);
		lcd_putstring("C");
		_delay_ms(4000);
		check_adc();


		oblicz_pozycje_slonca(THIS_POSITION_LAT,THIS_POSITION_LON,1,2000+year,mou,day,hour,min,sec);


		if(min==cSunriseM && hour==cSunriseH && sec<=30){  //  wsch�d
			open_door();
		}
		if(min==cSunsetM && hour==(cSunsetH + 1) && sec<=30){  //  zach�d
			close_door();
		}

		if(hour>=5&&hour<=6){
			val =  (hour-5)*60 + min;
			val1 = map(val,0,119,0,255);
			wl_pwm(val1);
		}
		else if(hour>=7&&hour<=18){
			if(adc>(adc_res + 25)){
				wyl_pwm_i_ustaw_stan(1);
				PWM_LED_OFF;
			}
			if(adc<(adc_res - 25)){
				wyl_pwm_i_ustaw_stan(0);
				PWM_LED_ON;
			}
			if(adc>(adc_res-25) && adc<(adc_res+25)){
			}
		}

		else if(hour>=20&&hour<=21){
			val =  (hour-19)*60 + min;
			val1 = map(val,0,119,255,0);
			wl_pwm(val1);
		}
		else{
			wyl_pwm_i_ustaw_stan(0);
		}
		_delay_ms(200);
	}
}
