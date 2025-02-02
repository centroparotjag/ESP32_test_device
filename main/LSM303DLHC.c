/*
 * LSM303DLHC.c
 *
 *  Created on: 1 лют. 2025 р.
 *      Author: centr
 */

#include "LSM303DLHC.h"
#include "foo_I2C.h"
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <sys/_intsup.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>

#define ADDR_LSM303_ACCEL	0x32
#define ADDR_LSM303_MAGN	0x3C

void config_LSM303 (void){
	write_byte_BMP180_LSM303 (ADDR_LSM303_ACCEL,  0x20, 0x27);
	write_byte_BMP180_LSM303 (ADDR_LSM303_ACCEL,  0x23, 0x00);
	
	write_byte_BMP180_LSM303 (ADDR_LSM303_MAGN,  0x02, 0x00);
	write_byte_BMP180_LSM303 (ADDR_LSM303_MAGN,  0x00, 0x88);				// 9c 10
	write_byte_BMP180_LSM303 (ADDR_LSM303_MAGN,  0x01, 0x20);
	
	vTaskDelay(10/portTICK_PERIOD_MS);		// 10ms;
	
}

void read_LSM303_acell (int16_t* acell_X, int16_t* acell_Y, int16_t* acell_Z){
	uint8_t data[6]={0};
	
	for (int i=0; i<6; i++){
		 data[i] = read_byte_BMP180_LSM303 (ADDR_LSM303_ACCEL, 0x28+i);
	}

	*acell_X = (data[1] << 8) + data[0];
	*acell_Y = (data[3] << 8) + data[2];
	*acell_Z = (data[5] << 8) + data[4];
}

void read_LSM303_magn (int16_t* magn_X, int16_t* magn_Y, int16_t* magn_Z){
	uint8_t data[8]={0};
	
	for (int i=0; i<6; i++){
		 data[i] = read_byte_BMP180_LSM303 (ADDR_LSM303_MAGN, 0x03+i);
	}
	
	*magn_X = (data[0] << 8) + data[1];
	*magn_Z = (data[2] << 8) + data[3];
	*magn_Y = (data[4] << 8) + data[5];
	 
	//---- T !??? -----------------------------------------
//	uint16_t T = 0;          
//	data[0] = read_byte_BMP180_LSM303 (ADDR_LSM303_MAGN, 0x31); 
//	data[1] = read_byte_BMP180_LSM303 (ADDR_LSM303_MAGN, 0x32);               
//	T = (data[0] << 4) + (data[1]>>4) ;
//	printf ("T LSM303 = %d\r\n", T/8);
}

void get_LSM303 (int16_t* a_X, int16_t* a_Y, int16_t* a_Z, int16_t* m_X, int16_t* m_Y, int16_t* m_Z){
	int16_t acell_X,  acell_Y,  acell_Z, magn_X, magn_Y, magn_Z;

	read_LSM303_acell (&acell_X, &acell_Y, &acell_Z);
	read_LSM303_magn (&magn_X, &magn_Y, &magn_Z);
	
	*a_X = acell_X;
	*a_Y = acell_Y;
	*a_Z = acell_Z;
	
	*m_X = magn_X;
	*m_Y = magn_Y;
	*m_Z = magn_Z;
}

void LSM303_print(void){
	int16_t a_X, a_Y, a_Z, m_X, m_Y, m_Z;
	
	get_LSM303 (&a_X, &a_Y, &a_Z, &m_X, &m_Y, &m_Z);
	printf ("Accelerometr X=%d, Y=%d, Z=%d\r\n", a_X, a_Y, a_Z);
	printf ("Magnetometer X=%d, Y=%d, Z=%d\r\n", m_X, m_Y, m_Z);
	
	float heading = (atan2(m_X, m_Y) * 180) / M_PI;

	if (heading < 0) {
		heading = 360 + heading;
	}
	printf ("Compass %0.1f\r\n", heading);
}
