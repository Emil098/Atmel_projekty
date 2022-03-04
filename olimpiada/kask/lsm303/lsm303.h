/*
 * lsm303.h
 *
 *  Created on: 18 gru 2017
 *      Author: Dv8
 */

#ifndef LSM303_LSM303_H_
#define LSM303_LSM303_H_
#include <avr/io.h>
#include <util/delay.h>
#include "../i2c/i2c.h"

#define OUT_X_L_A 0x28
#define OUT_X_H_A 0x29
#define OUT_Y_L_A 0x2A
#define OUT_Y_H_A 0x2B
#define OUT_Z_L_A 0x2C
#define OUT_Z_H_A 0x2D

#define MagWrite_Addr 0x3C
#define MagRead_Addr 0x3D
#define AccWrite_Addr 0x32
#define AccRead_Addr 0x33


#define CTRL_REG1_A 0x20
#define CTRL_REG2_A 0x21
#define CTRL_REG3_A 0x22
#define CTRL_REG4_A 0x23
#define CTRL_REG5_A 0x24
#define CTRL_REG6_A 0x25
#define CRA_REG_M 0x00
#define CRB_REG_M 0x01
#define MR_REG_M 0x02

float lsm303_heading(void);
uint16_t lsm303_read_y(void);
uint16_t lsm303_read_x(void);
uint16_t lsm303_read_z(void);
void lsm303_writeAccReg(uint8_t addr,uint8_t data);
void lsm303_writeMagReg(uint8_t addr,uint8_t data);
uint8_t lsm303_readAccReg(uint8_t addr);
uint8_t lsm303_readMagReg(uint8_t addr);
void lsm303_init(void);

#endif /* LSM303_LSM303_H_ */
