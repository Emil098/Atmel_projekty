/*
 * lsm303.c
 *
 *  Created on: 18 gru 2017
 *      Author: Dv8
 */
#include "lsm303.h"
#include <avr/io.h>
#include <util/delay.h>
#include "../i2c/i2c.h"
#include "math.h"

float lsm303_heading(void){  // aktualny k¹t kompasu
	uint16_t x,y;
	x = lsm303_read_x();
	y = lsm303_read_y();
	float PI = 3.14;
	// Obliczenie kierunku (rad)
	float heading = atan2(y,x);
	  // Ustawienie kata deklinacji dla Lublina 6'2E (positive)
	  // Formula: (deg + (min / 60.0)) / (180 / M_PI);
	  float declinationAngle = (6.0 + (2.0 / 60.0)) / (180 / M_PI);
	  heading += declinationAngle;
	  // Korekta katow
	  if (heading < 0)heading += 2 * PI;

	  if (heading > 2 * PI)heading -= 2 * PI;
	  // Zamiana radianow na stopnie
	  float headingDegrees = heading * 180/M_PI;
	return headingDegrees;
}
uint16_t lsm303_read_x(void){  // wektor x
	uint8_t x_l,x_h;
	uint16_t out_x;
	x_l = lsm303_readMagReg(OUT_X_L_A);
	x_h = lsm303_readMagReg(OUT_X_H_A);
	out_x = (x_l + (x_h<<8));
	return out_x;
}
uint16_t lsm303_read_y(void){  // wektor y
	uint8_t y_l,y_h;
	uint16_t out_y;
	y_l = lsm303_readMagReg(OUT_Y_L_A);
	y_h = lsm303_readMagReg(OUT_Y_H_A);
	out_y = (y_l + (y_h<<8));
	return out_y;
}
uint16_t lsm303_read_z(void){  // wektor z
	uint8_t z_l,z_h;
	uint16_t out_z;
	z_l = lsm303_readMagReg(OUT_Z_L_A);
	z_h = lsm303_readMagReg(OUT_Z_H_A);
	out_z = (z_l + z_h)/2;
	return out_z;
}
void lsm303_writeAccReg(uint8_t addr,uint8_t data){ // zapis do rejestrów akcelerometru
	i2c_start();
	i2c_send_byte(AccWrite_Addr);
	i2c_send_byte(addr);
	i2c_send_byte(data);
	i2c_stop();
}
uint8_t lsm303_readAccReg(uint8_t addr){  // odczyt z rejestrów akcelerometru
	i2c_start();
	i2c_send_byte(AccWrite_Addr);
	i2c_send_byte(addr);
	i2c_start();
	i2c_requestFrom(AccRead_Addr);
	uint8_t ret=i2c_read_byte(0);
	i2c_stop();
	return ret;
}
void lsm303_writeMagReg(uint8_t addr,uint8_t data){ // zapis do rejestrów magnetometru
	i2c_start();
	i2c_send_byte(MagWrite_Addr);
	i2c_send_byte(addr);
	i2c_send_byte(data);
	i2c_stop();
}
uint8_t lsm303_readMagReg(uint8_t addr){  // odczyt z rejestrów magnetometru
	i2c_start();
	i2c_send_byte(MagWrite_Addr);
	i2c_send_byte(addr);
	i2c_start();
	i2c_requestFrom(MagRead_Addr);
	uint8_t ret=i2c_read_byte(0);
	i2c_stop();
	return ret;
}
void lsm303_init(void){  // rozpoczêcie odczytu ustawienie trybu pracy
	i2c_init();
	lsm303_writeAccReg(CTRL_REG4_A, 0x08);
	lsm303_writeAccReg(CTRL_REG1_A, 0x47);
	lsm303_writeMagReg(CRA_REG_M, 0x0C);
	lsm303_writeMagReg(CRB_REG_M, 0x20);
	lsm303_writeMagReg(MR_REG_M, 0x00);
}
