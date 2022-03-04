/*
 * calc.c
 *
 * Created: 2015-08-18 12:50:24
 *  Author: admin
 */ 

#include <math.h>
#include <stdlib.h>
#include "calc.h"

#define PIdiv180  (flt)0.017453292519943295769236907684886
                                
/*static const u16 dayofyear[] = {  0, //styczeñ
                                 31, //luty
                                 59, //marzec
                                 90, //kwiecieñ
                                120, //maj
                                151, //czerwiec
                                181, //lipiec
                                212, //sierpieñ
                                243, //wrzesieñ
                                273, //paŸdziernik
                                304, //listopad
                                334};//grudzieñ  */
                                
flt cDeclination = 0; //w stopniach
flt cEquation = 0;    //w minutach
flt cAltitude = 0;    //w stopniach
flt cAzimuth = 0;     //w stopniach
u8 cSolarH = 0, cSolarM = 0;
u8 cSunriseH = 0, cSunriseM = 0; //wschód i zachód w czasie lokalnym! (nie w UTC)
u8 cSunriseOK = 0;
u8 cSunsetH = 0, cSunsetM = 0;
u8 cSunsetOK = 0;

static const u8 numdays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

u8 isLeapYear(u16 yr)
{
  return ((yr % 4 == 0 && yr % 100 != 0) || yr % 400 == 0);
}

/*inline*/ flt degToRad(flt angleDeg)
{
  return ((M_PI * angleDeg) / 180.0);
}

/*inline*/ flt radToDeg(flt angleRad)
{
  return ((180.0 * angleRad) / M_PI);
}

//mon: miesi¹c liczony od 1
//day: dzieñ liczony od 1
//year: rok 4-cyfrowo
flt getJD(u8 mon, u8 day, u16 year)
{             
  if ( (isLeapYear(year)) && (mon == 2) ) {
    if (day > 29) {
      day = 29;
    }
  } else {
    if (day > numdays[mon-1]) {
      day = numdays[mon-1];
    }
  }
  if (mon <= 2) {
    year -= 1;
    mon += 12;
  }
  u8 A = year / 100;
  s8 B = 2 - A + A/4;
  flt f = floor(365.25*((flt)year + 4716.0)) + floor(30.6001*((flt)mon+1.0));
  flt JD = f + day + B - 1524.5;
  return JD;
}

//czas lokalny w minutach
//hr: godzina w systemie 24-godzinnym
//NIE UWZGLÊDNIA ZMIANY CZASU! - Trzeba podaæ godzinê tak, jakby nie istnia³a zmiana czasu (czyli czas ziomwy)
flt getTimeLocal(u8 hr, u8 mn, u8 sc)
{
  return (hr * 60 + mn + sc/60.0);
} 

flt calcTimeJulianCent(flt jd)
{
  flt T = ((jd - 2451545.0) / 36525.0);
  return T;
} 

flt calcMeanObliquityOfEcliptic(flt t)
{
  flt seconds = 21.448 - t*(46.8150 + t*(0.00059 - t*(0.001813)));
  flt e0 = 23.0 + (26.0 + (seconds/60.0))/60.0;
  return e0;		// in degrees
} 
 
flt calcObliquityCorrection(flt t)
{
  flt e0 = calcMeanObliquityOfEcliptic(t); //mo¿e byæ = 0, nie mo¿e byæ NaN
  flt omega = 125.04 - 1934.136 * t;
  flt e = e0 + 0.00256 * cos(degToRad(omega));
  return e;		// in degrees
} 

flt calcGeomMeanLongSun(flt t)
{
  flt L0 = 280.46646 + t * (36000.76983 + t*(0.0003032));
  while(L0 > 360.0)
  {
    L0 -= 360.0;
  }
  while(L0 < 0.0)
  {
    L0 += 360.0;
  }
  return L0;		// in degrees
} 

flt calcEccentricityEarthOrbit(flt t)
{
  flt e = 0.016708634 - t * (0.000042037 + 0.0000001267 * t);
  return e;		// unitless
} 

flt calcGeomMeanAnomalySun(flt t)
{
  flt M = 357.52911 + t * (35999.05029 - 0.0001537 * t);
  return M;		// in degrees
} 
 
flt calcEquationOfTime(flt t)
{
  flt epsilon = calcObliquityCorrection(t); //mo¿e byæ 0, nie mo¿e byæ NaN
  flt l0 = calcGeomMeanLongSun(t);          //mo¿e byæ 0, nie moze byæ NaN
  flt e = calcEccentricityEarthOrbit(t);    //mo¿e byæ 0, nie mo¿e byæ NaN
  flt m = calcGeomMeanAnomalySun(t);        //mo¿e byæ 0, nie mo¿e byæ NaN

  flt y = tan(degToRad(epsilon)/2.0);       //mo¿e byæ nieskoñczonoœæ (albo bardzo du¿a liczba)
  y *= y;                                   

  flt sin2l0 = sin(2.0 * degToRad(l0));
  flt sinm   = sin(degToRad(m));
  flt cos2l0 = cos(2.0 * degToRad(l0));
  flt sin4l0 = sin(4.0 * degToRad(l0));
  flt sin2m  = sin(2.0 * degToRad(m));

  flt Etime = y * sin2l0 - 2.0 * e * sinm + 4.0 * e * y * sinm * cos2l0 - 0.5 * y * y * sin4l0 - 1.25 * e * e * sin2m;
  return radToDeg(Etime)*4.0;	// in minutes of time
} 

flt calcSunEqOfCenter(flt t)
{
  flt m = calcGeomMeanAnomalySun(t);   //mo¿e byæ 0, nie mo¿e byæ NaN
  flt mrad = degToRad(m);
  flt sinm = sin(mrad);
  flt sin2m = sin(mrad+mrad);
  flt sin3m = sin(mrad+mrad+mrad);
  flt C = sinm * (1.914602 - t * (0.004817 + 0.000014 * t)) + sin2m * (0.019993 - 0.000101 * t) + sin3m * 0.000289;
  return C;		// in degrees
}

flt calcSunTrueLong(flt t)
{
  flt l0 = calcGeomMeanLongSun(t);  //mo¿e byæ 0, nie moze byæ NaN
  flt c = calcSunEqOfCenter(t);     //mo¿e byæ 0, nie moze byæ NaN
  flt O = l0 + c;
  return O;		// in degrees
} 

flt calcSunApparentLong(flt t)
{
  flt o = calcSunTrueLong(t);  //mo¿e byæ 0, nie moze byæ NaN
  flt omega = 125.04 - 1934.136 * t;
  flt lambda = o - 0.00569 - 0.00478 * sin(degToRad(omega));
  return lambda;		// in degrees
}

flt calcSunDeclination(flt t)
{
  flt e = calcObliquityCorrection(t);  //mo¿e byæ 0, nie mo¿e byæ NaN
  flt lambda = calcSunApparentLong(t); //mo¿e byæ 0, nie moze byæ NaN

  flt sint = sin(degToRad(e)) * sin(degToRad(lambda));
  flt theta = radToDeg(asin(sint));  
  return theta;		// in degrees
} 

flt calcSunTrueAnomaly(flt t)
{
  flt m = calcGeomMeanAnomalySun(t);
  flt c = calcSunEqOfCenter(t);
  flt v = m + c;
  return v;		// in degrees
} 

flt calcSunRadVector(flt t)
{
  flt v = calcSunTrueAnomaly(t);
  flt e = calcEccentricityEarthOrbit(t);
  flt R = (1.000001018 * (1 - e * e)) / (1 + e * cos(degToRad(v)));  //TUTAJ MO¯E byæ dzielenie przez 0
  return R;		// in AUs
}

void MinutesToTime(flt minutes, u8 *h, u8 *m)
{
  if ( (minutes >= 0) && (minutes < 1440) ) {
    flt floatHour = minutes / 60.0;
    u8 hour = floor(floatHour);
    flt floatMinute = 60.0 * (floatHour - floor(floatHour));
    u8 minute = floor(floatMinute);
    flt floatSec = 60.0 * (floatMinute - floor(floatMinute));
    u8 second = floor(floatSec + 0.5);
    if (second > 59) {
      second = 0;
      minute += 1;
    }
    if (second >= 30) minute++;
    if (minute > 59) {
      minute = 0;
      hour += 1;
    }
    *h = hour;
    *m = minute;
    } else {
    *h = 0;
    *m = 0;
  }
}
 
void calcAzEl(flt T, flt localtime, flt latitude, flt longitude, flt zone)
{
  cEquation = calcEquationOfTime(T); //flt eqTime = calcEquationOfTime(T);
  cDeclination = calcSunDeclination(T); //flt theta  = calcSunDeclination(T)

  flt solarTimeFix = cEquation + 4.0 * longitude - 60.0 * zone;
  //flt earthRadVec = calcSunRadVector(T);
  flt trueSolarTime = localtime + solarTimeFix;
  MinutesToTime(trueSolarTime, &cSolarH, &cSolarM);
  while (trueSolarTime > 1440)
  {
    trueSolarTime -= 1440;
  }
  flt hourAngle = trueSolarTime / 4.0 - 180.0;
  if (hourAngle < -180)
  {
    hourAngle += 360.0;
  }
  flt haRad = degToRad(hourAngle);
  flt csz = sin(degToRad(latitude)) * sin(degToRad(cDeclination)) + cos(degToRad(latitude)) * cos(degToRad(cDeclination)) * cos(haRad);
  if (csz > 1.0)
  {
    csz = 1.0;
  } else if (csz < -1.0)
  {
    csz = -1.0;
  }
  flt zenith = radToDeg(acos(csz));
  flt azDenom = ( cos(degToRad(latitude)) * sin(degToRad(zenith)) );
  if (fabs(azDenom) > 0.001) {
    flt azRad = (( sin(degToRad(latitude)) * cos(degToRad(zenith)) ) - sin(degToRad(cDeclination))) / azDenom; 
    if (fabs(azRad) > 1.0) {
      if (azRad < 0) {
        azRad = -1.0;
      } else {
        azRad = 1.0;
      }
    }
    cAzimuth = 180.0 - radToDeg(acos(azRad));
    if (hourAngle > 0.0) {
      cAzimuth = -cAzimuth;
    }
  } else {
    if (latitude > 0.0) {
      cAzimuth = 180.0;
    } else {
      cAzimuth = 0.0;
    }
  }
  if (cAzimuth < 0.0) {
    cAzimuth += 360.0;
  }
  flt exoatmElevation = 90.0 - zenith;

  // Atmospheric Refraction correction

  flt refractionCorrection;
  
  if (exoatmElevation > 85.0) {
    refractionCorrection = 0.0;
  } else {
    flt te = tan(degToRad(exoatmElevation));
    if (exoatmElevation > 5.0) {
      refractionCorrection = 58.1 / te - 0.07 / (te*te*te) + 0.000086 / (te*te*te*te*te);  //TUTAJ MO¯E byæ dzielenie przez 0
    } else if (exoatmElevation > -0.575) {
      refractionCorrection = 1735.0 + exoatmElevation * (-518.2 + exoatmElevation * (103.4 + exoatmElevation * (-12.79 + exoatmElevation * 0.711) ) );
    } else {
      refractionCorrection = -20.774 / te;  //TUTAJ MO¯E byæ dzielenie przez 0
    }
    refractionCorrection = refractionCorrection / 3600.0;
  }

  flt solarZen = zenith - refractionCorrection;
  
  cAltitude = 90.0 - solarZen;

  /*if ((output) && (solarZen > 108.0) ) {
    document.getElementById("azbox").value = "dark"
    document.getElementById("elbox").value = "dark"
  } else if (output) {
    document.getElementById("azbox").value = Math.floor(azimuth*100 +0.5)/100.0
    document.getElementById("elbox").value = Math.floor((90.0-solarZen)*100+0.5)/100.0
    if (document.getElementById("showae").checked) {
      showLineGeodesic2("azimuth", "#ffff00", azimuth)
    }
  }
  return (azimuth) */ 
} 

flt calcHourAngleSunrise(flt lat, flt solarDec)
{
  flt latRad = degToRad(lat);
  flt sdRad  = degToRad(solarDec);
  flt HAarg = cos(degToRad(90.833))/(cos(latRad)*cos(sdRad)) - tan(latRad) * tan(sdRad);
  flt HA = acos(HAarg);
  return HA;		// in radians (for sunset, use -HA)
}

flt calcSunriseSetUTC(u8 rise, flt JD, flt latitude, flt longitude)   //jak ta funkcja mo¿e zwróciæ coœ innego, ni¿ liczba?
{
  flt t = calcTimeJulianCent(JD);        //odejmowanie i dzielenie, mo¿e byæ = 0, nie mo¿e byæ b³êdu
  flt eqTime = calcEquationOfTime(t);    //tangens, chyba mo¿e zwróciæ NAN lub INFINITY
  flt solarDec = calcSunDeclination(t);  //asin, chyba mo¿e byæ NAN
  flt hourAngle = calcHourAngleSunrise(latitude, solarDec); //tan i acos, chyba mo¿e byæ NAN lub INFINITY
  if (!rise) hourAngle = -hourAngle;
  flt delta = longitude + radToDeg(hourAngle);
  flt timeUTC = 720 - (4.0 * delta) - eqTime;	// in minutes
  return timeUTC;
}

//zwraca 1 jeœli uda³o siê obliczyæ, 0 jeœli nie
u8 calcSunriseSet(u8 rise, flt JD, flt latitude, flt longitude, flt timezone, flt *timeLocal, flt *jday)
// rise = 1 for sunrise, 0 for sunset
{
  //var id = ((rise) ? "risebox" : "setbox")
  flt timeUTC = calcSunriseSetUTC(rise, JD, latitude, longitude);
  flt newTimeUTC = calcSunriseSetUTC(rise, JD + timeUTC/1440.0, latitude, longitude);
  
  *jday = 0;
  
  if (isfinite(newTimeUTC)) {  //chyba newTimeUTC mo¿e byæ NAN lub INIFINITY
    *timeLocal = newTimeUTC + (timezone * 60.0);
    //timeLocal += ((dst) ? 60.0 : 0.0);
    if ( (*timeLocal >= 0.0) && (*timeLocal < 1440.0) ) {
      //document.getElementById(id).value = timeString(timeLocal,2)     
      return 1;
    } else  {
      *jday = JD;
      u8 increment = ((*timeLocal < 0) ? 1 : -1);
      while ((*timeLocal < 0.0)||(*timeLocal >= 1440.0)) {
        *timeLocal += increment * 1440.0;
        *jday -= increment;
      }
      //document.getElementById(id).value = timeDateString(jday,timeLocal)
      return 1;
    }
  } else { // no sunrise/set found
    return 0;
    /*fly doy = calcDoyFromJD(JD)
    if ( ((latitude > 66.4) && (doy > 79) && (doy < 267)) ||
    ((latitude < -66.4) && ((doy < 83) || (doy > 263))) )
    {   //previous sunrise/next sunset
      if (rise) { // find previous sunrise
        jdy = calcJDofNextPrevRiseSet(0, rise, JD, latitude, longitude, timezone, dst)
        } else { // find next sunset
        jdy = calcJDofNextPrevRiseSet(1, rise, JD, latitude, longitude, timezone, dst)
      }
      document.getElementById(((rise)? "risebox":"setbox")).value = dayString(jdy,0,3)
      } else {   //previous sunset/next sunrise
      if (rise == 1) { // find previous sunrise
        jdy = calcJDofNextPrevRiseSet(1, rise, JD, latitude, longitude, timezone, dst)
        } else { // find next sunset
        jdy = calcJDofNextPrevRiseSet(0, rise, JD, latitude, longitude, timezone, dst)
      }
      document.getElementById(((rise)? "risebox":"setbox")).value = dayString(jdy,0,3)
    }  */
  }
}  

//Lat = Latitude - szerokoœæ geograficzna w stopniach (N = dodatnie, S = ujemne)
//Lon = Longitude - d³ugoœæ geograficzna w stopniach  (E = dodatnie, W = ujemne)
//tz = timezone - strefa czasowa (+ iloœæ godzin od GMT)
//mon - miesiac (1 - 12)
//d - dzieñ (1 - 31)
//h - godzina
//min - minuta
//NIE UWZGLÊDNIA ZMIANY CZASU! - Trzeba podaæ godzinê tak, jakby nie istnia³a zmiana czasu (czyli czas ziomwy)
void oblicz_pozycje_slonca(flt Lat, flt Lon, flt tz, u16 y, u8 mon, u8 d, u8 h, u8 min, u8 s) 
{
  
  //Jakieœ próby niedzia³aj¹ce:
  /*flt deklinacja = 23.45 * sin(PIdiv180 * ((flt)360 * ((flt)(284+dzien_roku) / (flt)365)));
  flt czas_sloneczny =  
  flt omega = 15 * czas_sloneczny - 180;  
  flt wysokosc = asin( (sin(fi)*sin(deklinacja) + cos(fi)*cos(deklinacja)*cos(omega) ) / PIdiv180;*/
  
  
  //Ze strony http://wiki.naturalfrequency.com/wiki/Solar_Position_Calculator: 
 /* // Location in radians.
  flt fLatitude;
  flt fLongitude;
  flt fTimeZone;

  // Calculated data.
  flt fDifference;
  flt fDeclination;
  flt fEquation;

  // Solar information.
  flt fLocalTime;
  flt fSolarTime;
  flt fAltitude;
  flt fAzimuth;
  flt fSunrise;
  flt fSunset;

  // Integer values.
  u16 iJulianDate;

  ////////////////////////////////////////////////////////////
  // READ INPUT DATA
  ////////////////////////////////////////////////////////////

  // Get location data.
  fLatitude = Lat * PIdiv180; //parseFloat(document.theForm.inputLatitude.value) * (Math.PI/180.0);
  fLongitude = Lon * PIdiv180; //parseFloat(document.theForm.inputLongitude.value) * (Math.PI/180.0);
  fTimeZone =  tz * 15 * PIdiv180; //parseFloat(document.theForm.inputTimeZone.options[document.theForm.inputTimeZone.selectedIndex].value) * 15 * (Math.PI/180.0);
  

  // Get julian date.
  iJulianDate = dayofyear[mon] //parseInt(document.theForm.inputMonth.options[document.theForm.inputMonth.selectedIndex].value)
              + d;             //+ parseInt(document.theForm.inputDate.options[document.theForm.inputDate.selectedIndex].value);

  // Get local time value.
  fLocalTime = h*100 + min; //parseFloat(parse_time(document.theForm.inputTime.value));

  // Check for military time (2400).
  if (fLocalTime > 100) {
    fLocalTime /= 100.0;
    u8 t = floor(fLocalTime);
    u8 m = round((fLocalTime - t) * 100.0);
    fLocalTime = t + (m / 60.0); //wynik: godzina w czêœci ca³kowitej i u³amek godziny w czêœci u³amkowej
  }


  ////////////////////////////////////////////////////////////
  // CALCULATE SOLAR VALUES
  ////////////////////////////////////////////////////////////

  // Preliminary check.
  if (iJulianDate > 365) iJulianDate -= 365;
  if (iJulianDate < 0) iJulianDate += 365;

  // Secondary check of julian date.
  if (iJulianDate > 365) iJulianDate = 365;
  if (iJulianDate < 1) iJulianDate = 1;

  // Calculate solar declination as per Carruthers et al.
  flt t = 2 * M_PI * ((iJulianDate - 1) / 365.0);

  fDeclination = (0.322003
  - 22.971 * cos(t)
  - 0.357898 * cos(2*t)
  - 0.14398 * cos(3*t)
  + 3.94638 * sin(t)
  + 0.019334 * sin(2*t)
  + 0.05928 * sin(3*t)
  );

  // Convert degrees to radians.
  if (fDeclination > 89.9) fDeclination = 89.9;
  if (fDeclination < -89.9) fDeclination = -89.9;

  // Convert to radians.
  fDeclination = fDeclination * PIdiv180;

  // Calculate the equation of time as per Carruthers et al.
  t = (279.134 + 0.985647 * iJulianDate) * PIdiv180;

  fEquation = (5.0323
  - 100.976 * sin(t)
  + 595.275 * sin(2*t)
  + 3.6858 * sin(3*t)
  - 12.47 * sin(4*t)
  - 430.847 * cos(t)
  + 12.5024 * cos(2*t)
  + 18.25 * cos(3*t)
  );

  // Convert seconds to hours.
  fEquation = fEquation / 3600.00;

  // Calculate difference (in minutes) from reference longitude.
  fDifference = (((fLongitude - fTimeZone) * 180/M_PI) * 4) / 60.0;

  // Convert solar noon to local noon.
  flt local_noon = 12.0 - fEquation - fDifference;

  // Calculate angle normal to meridian plane. (M_PI_2 = PI/2)
  if (fLatitude > (0.99 * M_PI_2)) fLatitude = (0.99 * M_PI_2);
  if (fLatitude < -(0.99 * M_PI_2)) fLatitude = -(0.99 * M_PI_2);

  flt test = -tan(fLatitude) * tan(fDeclination);

  if (test < -1) t = acos(-1.0) / (15 * PIdiv180);
  // else if (test > 1) t = acos(1.0) / (15 * (Math.PI / 180.0)); ### Correction - missing 'Math.acos'
  else if (test > 1) t = acos(1.0) / (15 * PIdiv180);
  else t = acos(-tan(fLatitude) * tan(fDeclination)) / (15 * PIdiv180);

  // Sunrise and sunset.
  fSunrise = local_noon - t;
  fSunset  = local_noon + t;
    
  // Check validity of local time.
  if (fLocalTime > fSunset) fLocalTime = fSunset;
  if (fLocalTime < fSunrise) fLocalTime = fSunrise;
  if (fLocalTime > 24.0) fLocalTime = 24.0;
  if (fLocalTime < 0.0) fLocalTime = 0.0;

  // Caculate solar time.
  fSolarTime = fLocalTime + fEquation + fDifference;

  // Calculate hour angle.
  flt fHourAngle = (15 * (fSolarTime - 12)) * PIdiv180;

  // Calculate current altitude.
  t = (sin(fDeclination) * sin(fLatitude)) + (cos(fDeclination) * cos(fLatitude) * cos(fHourAngle));
  fAltitude = asin(t);

  // Original calculation of current azimuth - LEAVE COMMENTED AS DOES NOT WORK CORRECTLY.
  // t = (Math.cos(fLatitude) * Math.sin(fDeclination)) - (Math.cos(fDeclination) * Math.sin(fLatitude) * Math.cos(fHourAngle));
  // fAzimuth = Math.acos(t / Math.cos(fAltitude));

  // FIX:
  // ##########################################
  // Need to do discrete quadrant checking.

  // Calculate current azimuth.
  t = (sin(fDeclination) * cos(fLatitude))
  - (cos(fDeclination) * sin(fLatitude)
  *  cos(fHourAngle));

  flt sin1, cos2;
  
  // Avoid division by zero error.
  if (fAltitude < M_PI_2) {
    sin1 = (-cos(fDeclination) * sin(fHourAngle)) / cos(fAltitude);
    cos2 = t / cos(fAltitude);
  }

  else {
    sin1 = 0.0;
    cos2 = 0.0;
  }

  // Some range checking.
  if (sin1 > 1.0) sin1 = 1.0;
  if (sin1 < -1.0) sin1 = -1.0;
  if (cos2 < -1.0) cos2 = -1.0;
  if (cos2 > 1.0) cos2 = 1.0;

  // Calculate azimuth subject to quadrant.
  if (sin1 < -0.99999) fAzimuth = asin(sin1);

  else if ((sin1 > 0.0) && (cos2 < 0.0)) {
    if (sin1 >= 1.0) fAzimuth = -(M_PI_2);
    else fAzimuth = M_PI_2 + (M_PI_2 - asin(sin1));
  }

  else if ((sin1 < 0.0) && (cos2 < 0.0)) {
    if (sin1 <= -1.0) fAzimuth = M_PI_2;
    else fAzimuth = -(M_PI_2) - (M_PI_2 + asin(sin1));
  }

  else fAzimuth = asin(sin1);

  // A little last-ditch range check.
  if ((fAzimuth < 0.0) && (fLocalTime < 10.0)) {
    fAzimuth = -fAzimuth;
  }

  ////////////////////////////////////////////////////////////
  // FORMAT OUTPUT DATA
  ////////////////////////////////////////////////////////////

  // Output declination.
  cDeclination = round((fDeclination * (180.0 / M_PI)) * 1000) / 1000.0;

  // Output equation of time.
  cEquation = round((fEquation * 60.0) * 1000) / 1000.0;

  // Output altitude value.
  cAltitude = round(fAltitude * (180.0/M_PI) * 100) / 100.0;

  // Output aziumth value.
  cAzimuth = round(fAzimuth * (180.0/M_PI) * 100) / 100.0;

  // Output solar time.
  cSolarH = floor(fSolarTime);
  cSolarM = floor((fSolarTime - cSolarH) * 60.0);

  // Output sunrise time.
  cSunriseH = floor(fSunrise);
  cSunriseM = floor((fSunrise - cSunriseH) * 60.0);

  // Output sunset time.
  cSunsetH = floor(fSunset);
  cSunsetM = floor((fSunset - cSunsetH) * 60.0); */
 
 
  //Ze strony http://www.esrl.noaa.gov/gmd/grad/solcalc/ (main.js)




 /*function calcJDFromJulianCent(t)
 {
   var JD = t * 36525.0 + 2451545.0
   return JD
 } */



 /*function calcDoyFromJD(jd)
 {
   var z = Math.floor(jd + 0.5);
   var f = (jd + 0.5) - z;
   if (z < 2299161) {
     var A = z;
     } else {
     alpha = Math.floor((z - 1867216.25)/36524.25);
     var A = z + 1 + alpha - Math.floor(alpha/4);
   }
   var B = A + 1524;
   var C = Math.floor((B - 122.1)/365.25);
   var D = Math.floor(365.25 * C);
   var E = Math.floor((B - D)/30.6001);
   var day = B - D - Math.floor(30.6001 * E) + f;
   var month = (E < 14) ? E - 1 : E - 13;
   var year = (month > 2) ? C - 4716 : C - 4715;

   var k = (isLeapYear(year) ? 1 : 2);
   var doy = Math.floor((275 * month)/9) - k * Math.floor((month + 9)/12) + day -30;
   return doy;
 }*/














 /*function calcSunRtAscension(t)
 {
   var e = calcObliquityCorrection(t);
   var lambda = calcSunApparentLong(t);
   var tananum = (Math.cos(degToRad(e)) * Math.sin(degToRad(lambda)));
   var tanadenom = (Math.cos(degToRad(lambda)));
   var alpha = radToDeg(Math.atan2(tananum, tanadenom));
   return alpha;		// in degrees
 } */



 



 /*function isNumber(inputVal)
 {
   var oneDecimal = false;
   var inputStr = "" + inputVal;
   for (var i = 0; i < inputStr.length; i++)
   {
     var oneChar = inputStr.charAt(i);
     if (i == 0 && (oneChar == "-" || oneChar == "+"))
     {
       continue;
     }
     if (oneChar == "." && !oneDecimal)
     {
       oneDecimal = true;
       continue;
     }
     if (oneChar < "0" || oneChar > "9")
     {
       return false;
     }
   }
   return true;
 } */


 /*function zeroPad(n, digits) {
   n = n.toString();
   while (n.length < digits) {
     n = '0' + n;
   }
   return n;
 } */

 /*function readTextBox(inputId, numchars, intgr, pad, min, max, def)
 {
   var number = document.getElementById(inputId).value.substring(0,numchars)
   if (intgr) {
     number = Math.floor(parseFloat(number))
     } else {  // float
     number = parseFloat(number)
   }
   if (number < min) {
     number = min
     } else if (number > max) {
     number = max
     } else if (number.toString() == "NaN") {
     number = def
   }
   if ((pad) && (intgr)) {
     document.getElementById(inputId).value = zeroPad(number,2)
     } else {
     document.getElementById(inputId).value = number
   }
   return number
 } */



 

 /*function calcSolNoon(jd, longitude, timezone, dst)
 {
   var tnoon = calcTimeJulianCent(jd - longitude/360.0)
   var eqTime = calcEquationOfTime(tnoon)
   var solNoonOffset = 720.0 - (longitude * 4) - eqTime // in minutes
   var newt = calcTimeJulianCent(jd + solNoonOffset/1440.0)
   eqTime = calcEquationOfTime(newt)
   solNoonLocal = 720 - (longitude * 4) - eqTime + (timezone*60.0)// in minutes
   if(dst) solNoonLocal += 60.0
   while (solNoonLocal < 0.0) {
     solNoonLocal += 1440.0;
   }
   while (solNoonLocal >= 1440.0) {
     solNoonLocal -= 1440.0;
   }
   document.getElementById("noonbox").value = timeString(solNoonLocal, 3)
 } */

 /*function dayString(jd, next, flag)
 {
   // returns a string in the form DDMMMYYYY[ next] to display prev/next rise/set
   // flag=2 for DD MMM, 3 for DD MM YYYY, 4 for DDMMYYYY next/prev
   if ( (jd < 900000) || (jd > 2817000) ) {
     var output = "error"
     } else {
     var z = Math.floor(jd + 0.5);
     var f = (jd + 0.5) - z;
     if (z < 2299161) {
       var A = z;
       } else {
       alpha = Math.floor((z - 1867216.25)/36524.25);
       var A = z + 1 + alpha - Math.floor(alpha/4);
     }
     var B = A + 1524;
     var C = Math.floor((B - 122.1)/365.25);
     var D = Math.floor(365.25 * C);
     var E = Math.floor((B - D)/30.6001);
     var day = B - D - Math.floor(30.6001 * E) + f;
     var month = (E < 14) ? E - 1 : E - 13;
     var year = ((month > 2) ? C - 4716 : C - 4715);
     if (flag == 2)
     var output = zeroPad(day,2) + " " + monthList[month-1].abbr;
     if (flag == 3)
     var output = zeroPad(day,2) + monthList[month-1].abbr + year.toString();
     if (flag == 4)
     var output = zeroPad(day,2) + monthList[month-1].abbr + year.toString() + ((next) ? " next" : " prev");
   }
   return output;
 } */

 /*function timeDateString(JD, minutes)
 {
   var output = timeString(minutes, 2) + " " + dayString(JD, 0, 2);
   return output;
 } */

 



 

 /*function calcJDofNextPrevRiseSet(next, rise, JD, latitude, longitude, tz, dst)
 {
   var julianday = JD;
   var increment = ((next) ? 1.0 : -1.0);

   var time = calcSunriseSetUTC(rise, julianday, latitude, longitude);
   while(!isNumber(time)){
     julianday += increment;
     time = calcSunriseSetUTC(rise, julianday, latitude, longitude);
   }
   var timeLocal = time + tz * 60.0 + ((dst) ? 60.0 : 0.0)
   while ((timeLocal < 0.0) || (timeLocal >= 1440.0))
   {
     var incr = ((timeLocal < 0) ? 1 : -1)
     timeLocal += (incr * 1440.0)
     julianday -= incr
   }
   return julianday;
 } */

 //function calculate() {
   flt jday = getJD(mon, d, y);
   flt tl = getTimeLocal(h, min, s);
   //var tz = readTextBox("zonebox", 5, 0, 0, -14, 13, 0)
   //ZOBACZ var dst = document.getElementById("dstCheckbox").checked
   flt total = jday + tl/1440.0 - tz/24.0;
   flt T = calcTimeJulianCent(total);
   //var lat = parseFloat(document.getElementById("latbox").value.substring(0,9))
   //var lng = parseFloat(document.getElementById("lngbox").value.substring(0,10))
   calcAzEl(T, tl, Lat, Lon, tz);
   //calcSolNoon(jday, lng, tz, dst)
   flt srss_jd, srss_tl;
   cSunriseOK = calcSunriseSet(1, jday, Lat, Lon, tz, &srss_tl, &srss_jd);
   if (cSunriseOK) MinutesToTime(srss_tl, &cSunriseH, &cSunriseM);
   cSunsetOK = calcSunriseSet(0, jday, Lat, Lon, tz, &srss_tl, &srss_jd);
   if (cSunsetOK) MinutesToTime(srss_tl, &cSunsetH, &cSunsetM); 
}  