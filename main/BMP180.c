/*
 * BMP180.c
 *
 *  Created on: 31 січ. 2025 р.
 *      Author: centr
 */
#include "BMP180.h"
#include "foo_I2C.h"
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <sys/_intsup.h>
#include <sys/types.h>
#include <unistd.h>

#define ADDR_BM180		0xEE 

extern int T_bmp180;
extern unsigned int P_bmp180;

void getPressure_BMP180 (void){
	uint8_t data[22];
	uint8_t c[2];
	uint8_t cp[3];
	
	//------------- soft reset --------------------
	write_byte_BMP180_LSM303 (ADDR_BM180, 0xE0, 0xB6);				// soft reset
	vTaskDelay(20/portTICK_PERIOD_MS);		// 20ms;
	
	//-------- read calibration data --------------
	read_data_BMP180_LSM303 (ADDR_BM180, 0xAA, data, 22);	
	short	AC1 = (data[0]<<8)  + data[1];
	short	AC2 = (data[2]<<8)  + data[3];
	short	AC3 = (data[4]<<8)  + data[5];
	ushort	AC4 = (data[6]<<8)  + data[7];
	ushort	AC5 = (data[8]<<8)  + data[9];
	ushort	AC6 = (data[10]<<8) + data[11];
	short	B1  = (data[12]<<8) + data[13];
	short	B2  = (data[14]<<8) + data[15];
	//short	MB  = (data[16]<<8) + data[17];
	short	MC  = (data[18]<<8) + data[19];
	short	MD  = (data[20]<<8) + data[21];
	
	//---- Read uncompensated temperature value ----
	write_byte_BMP180_LSM303 (ADDR_BM180, 0xF4, 0x2E);				// start convert T
	vTaskDelay(10/portTICK_PERIOD_MS);		// 10ms;
	read_data_BMP180_LSM303 (ADDR_BM180, 0xF6, c, 2);			// read T
	long UT = (c[0]<<8) + c[1];
	
	//---- Read uncompensated pressure value -----
	short oss = 3;											// oss = 0b11 ( 8 times oversampling ratio ) 
	write_byte_BMP180_LSM303 (ADDR_BM180,0xF4, 0x34+(oss<<6));	// start convert P
	vTaskDelay(30/portTICK_PERIOD_MS);		// 30ms;
	read_data_BMP180_LSM303 (ADDR_BM180, 0xF6, cp, 3);		// read P
	long UP = ((cp[0]<<16) + (cp[1]<<8) + cp[2]) >> (8-oss);
	
	//---------- calculate true temperature ------------------
	long x1 = (((long)UT - (long)AC6)*(long)AC5) >> 15;
 	long x2 = ((long)MC << 11)/(x1 + MD);
	long B5 = x1 + x2;
	long T = (B5+8) >> 4;
	T_bmp180 = (int)T;
	//printf("T = %lu;\r\n",  T);
	
	//---------- calculate true pressure ------------------
	long b6 = B5 - 4000;
	// Calculate B3
	x1 = (B2 * (b6 * b6)>>12)>>11;
	x2 = (AC2 * b6)>>11;
	long x3 = x1 + x2;
	long b3 = ((((AC1)*4 + x3)<<oss) + 2)>>2;
	// Calculate B4
	x1 = (AC3 * b6)>>13;
	x2 = (B1 * ((b6 * b6)>>12))>>16;
	x3 = ((x1 + x2) + 2)>>2;
	unsigned long b4 = (AC4 * (unsigned long)(x3 + 32768))>>15;
	unsigned long b7 = ((unsigned long)(UP - b3) * (50000>>oss));
	long p = (b7 < 0x80000000) ? (b7<<1)/b4 : (b7/b4)<<1;
	x1 = (p>>8) * (p>>8);
	x1 = (x1 * 3038)>>16;
	x2 = (-7357 * p)>>16;
	p += (x1 + x2 + 3791)>>4;
	P_bmp180 = (unsigned int)p;
	//printf("P_bmp180 = %lu;\r\n",  (long)p);
}

void BMP180_print_P_T (void){
	float p_180 = (float)P_bmp180/100;
	float t_180 = (float)T_bmp180/10;
	printf("BMP180  T = %0.1fC; P = %0.2fhPa; \r\n", t_180, p_180);
}



