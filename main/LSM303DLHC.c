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

extern int16_t accel_X; 
extern int16_t accel_Y;
extern int16_t accel_Z;
extern int16_t mag_X; 
extern int16_t mag_Y; 
extern int16_t mag_Z;
extern float compas;


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
}

void get_LSM303 (void){
	int16_t acell_X,  acell_Y,  acell_Z, magn_X, magn_Y, magn_Z;
	float heading;

	read_LSM303_acell (&acell_X, &acell_Y, &acell_Z);
	read_LSM303_magn (&magn_X, &magn_Y, &magn_Z);
	
	heading = (atan2(magn_X, magn_Y) * 180) / M_PI;
	if (heading < 0) { heading = 360 + heading; }
	
	if (magn_Z < 0) { heading = 360 - heading; }  // inverted compass upside down
	
	accel_X = acell_X;
	accel_Y = acell_Y;
	accel_Z = acell_Z;
	mag_X   = magn_X;
	mag_Y   = magn_Y;
	mag_Z   = magn_Z;
	compas  = heading;
} 

void LSM303_print(void){
	printf ("Accelerometr X=%d, Y=%d, Z=%d\r\n", accel_X, accel_Y, accel_Z);
	printf ("Magnetometer X=%d, Y=%d, Z=%d\r\n", mag_X, mag_Y, mag_Z);
	printf ("Compass %0.1f\r\n", compas);
}



