#include <avr/io.h>
#include <util/delay.h>
#include "hc05/hc05.h"
#include "i2c/i2c.h"
#include "lsm303/lsm303.h"
#include "uart/uart.h"

int val_difference,val_difference_from_recived,recv,tim_switch;
uint8_t state;
int main(){
	ws2812_init();
	lsm303_init();
	HC05_begin();
	val_difference = val_current();
	  recv=HC05_recive();      //odczyt z ramy roweru
	  while(recv<0||recv>360){
	     recv=HC05_recive();   //sprawdzenie poprawnoci danych odczytanych
	  }
	  val_difference_from_recived = recv; //zmienna od kttórej liczona jest różnica stopni tak aby na początku 2 kompasy wskazywaly 0stopni
	  for(int i=0;i<23;i++){
	     setWs(100,100,100,i);
	  }
	  ws2812_send();
	  noInterrupts();           // wylanczenie przerwania  TCCR1A = 0;
	  TCCR1B = 0;
	  TCNT1  = 0;

   OCR1A   = 31250;            // rejestr ustalenia częstotliwosć pracy czyli 2Hz 
   TCCR1B |= (1 << WGM12);   // CTC mode
   TCCR1B |= (1 << CS12);    // 256 prescaler
   TIMSK | = (1 << OCIE1A);  // wlączenie przerwania lokalnego
	  interrupts();
	  tim_switch=0;
	}
	ISR(TIMER1_COMPA_vect)          // czynnosci przy przerwaniu
	{
	  if(tim_switch==1){  // kierunkowskaz lewy miganie
	  if(state){
	  for(int i=0;i<48;i++){
	    if(i<8)setWs(150,100,0,i);
	    if(i<32&&i>=24)setWs(150,100,0,i);
	    else setWs(100,100,100,i);
	  }
	  ws2812_send();
	  }
	  else{
	    for(int i=0;i<48;i++){
	     setWs(100,100,100,i);
	  }
	  ws2812_send();
	  }
	  state = !state;
	}else if(tim_switch==2){  // kierunkowskaz prawy miganie
	  if(state){
	  for(int i=0;i<48;i++){
	    if(i>16&&i<24)setWs(150,100,0,i);
	    if(i>40)setWs(150,100,0,i);
	    else setWs(100,100,100,i);
	  }
	  ws2812_send();
	  }
	  else{
	    for(int i=0;i<48;i++){
	     setWs(100,100,100,i);
	  }
	  ws2812_send();
	  }
	  state = !state;
	}
	else{                 // brak kierunkowskazu
	  for(int i=0;i<48;i++){
	     setWs(100,100,100,i);
	  }
	  ws2812_send();
	}
	}
	void loop()
	{
	  int val_c = val_current();    // odczyt aktualnej wartosci z kasku
	  int val_at_0 = val_at_zero(val_c);  // przyrównanie do 0
	  recv=HC05_recive();			//odczyt z ramy roweru
	  while(recv<0||recv>360){
	     recv=HC05_recive();
	  }
	  int recv_at_0 = val_at_zero_recived(recv); // przyrównanie do 0
	  int dif = difference(val_at_0,recv_at_0);  // wyliczenie różnicy
	  if(dif>60){  //jesli wynosi +60 kierunkowskaz prawy
	    tim_switch=2;
	    if((recv_at_0+60)<360)turn_Right(recv_at_0+60); // ustawienie wartosci przy której program się resetuje
	    else turn_Right((recv_at_0+60)-360);
	  }
	  if(dif<(-60)){ //jesli różnica wynosi -60 kierunkowskaz prawy
	    tim_switch=1;
	    if((recv_at_0-60)>0)turn_Left(recv_at_0-60); // ustawienie wartosci przy której program się resetuje
	    else turn_Left(360-((recv_at_0-60)*(-1)));
	  }
	  _delay_ms(100);
	}
	void turn_Left(int reset_val){
	  tim_switch=1;
	  digitalWrite(2,1);
	  int recv_at_0;
	  back:
	  recv=HC05_recive();
	  while(recv<0||recv>360){
	     recv=HC05_recive();
	  }
	  recv_at_0 = val_at_zero_recived(recv);
	  int dif = difference(recv_at_0,reset_val);
	  if(dif>0)goto back;
	  tim_switch=0;
	  asm volatile("jmp 0");
	}
	void turn_Right(int reset_val){
	  int recv_at_0;
	  back:
	  recv=HC05_recive();
	  while(recv<0||recv>360){
	     recv=HC05_recive();
	  }
	  recv_at_0 = val_at_zero_recived(recv);
	  int dif = difference(reset_val,recv_at_0);
	  if(dif>0)goto back;
	  tim_switch=0;
	  asm volatile("jmp 0");
	}

	int difference(int kask,int kierownica){
	  int ret_val;
	  if(kask>kierownica&&(kask-kierownica)>0&&(kask-kierownica)<180)ret_val = kask-kierownica;
	  if(kask<kierownica&&(kierownica-kask)>0&&(kierownica-kask)<180)ret_val = (kierownica-kask)*(-1);
	  if(kask>kierownica&&(kask-kierownica)>0&&(kask-kierownica)>180)ret_val = (((kask-360)*(-1))+kierownica)*(-1);
	  if(kask<kierownica&&(kierownica-kask)>0&&(kierownica-kask)>180)ret_val = (((kierownica-360)*(-1))+kask);
	  if(kask==kierownica)ret_val=0;
	  return ret_val;
	}
	int val_at_zero_recived(int current){
	if(current >= val_difference_from_recived)return (current-val_difference_from_recived);
	if(current < val_difference_from_recived)return (360+(current-val_difference_from_recived));
	}
	int val_at_zero(int current){
	if(current >= val_difference){
		return (current-val_difference);
	}
	if(current < val_difference){
		return (360+(current-val_difference));
	}
	}
	int val_current(){
	  float heading = lsm303_heading();
	return heading;
	}
	void setWs(int r,int g,int b,int led_nr){
	red[led_nr] = r;
	green[led_nr] = g;
	blue[led_nr] = b;
	}

