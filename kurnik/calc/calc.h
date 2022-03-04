/*
 * calc.h
 *
 * Created: 2015-08-18 12:50:36
 *  Author: admin
 */ 


#ifndef CALC_H_
#define CALC_H_

#include "types.h"

#define flt long double

extern flt cDeclination; //w stopniach
extern flt cEquation;    //w minutach
extern flt cAltitude;    //w stopniach
extern flt cAzimuth;     //w stopniach
extern u8 cSolarH, cSolarM;
extern u8 cSunriseH, cSunriseM;
extern u8 cSunriseOK;
extern u8 cSunsetH, cSunsetM;
extern u8 cSunsetOK;

void oblicz_pozycje_slonca(flt Lat, flt Lon, flt tz, u16 y, u8 mon, u8 d, u8 h, u8 min, u8 s);


#endif /* CALC_H_ */