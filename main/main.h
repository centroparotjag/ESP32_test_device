/*
 * main.h
 *
 *  Created on: 3 лют. 2025 р.
 *      Author: centr
 */

#ifndef MAIN_MAIN_H_
#define MAIN_MAIN_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/unistd.h>
#include <unistd.h>
#include <hal/gpio_types.h>
#include "driver/gpio.h"
#include "rom/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "DS18B20.h"
#include "DHT.h"
#include "WS2812D.h"
#include "BMP180.h"
#include "config_GPIO.h"
#include "foo_I2C.h"
#include "statistics.h"
#include "LSM303DLHC.h"
#include "measurement_physical_quantities.h"

//-------- global ----------
float T_ds18b20 = 0; 
uint8_t Rh_dht11 = 0;
float T_dht11 = 0; 
int T_bmp180 = 0;
unsigned int P_bmp180 = 0;
int16_t accel_X = 0; 
int16_t accel_Y = 0;
int16_t accel_Z = 0;
int16_t mag_X = 0; 
int16_t mag_Y = 0; 
int16_t mag_Z = 0;
float compas = 0;
uint8_t tic = 0;



#endif /* MAIN_MAIN_H_ */
