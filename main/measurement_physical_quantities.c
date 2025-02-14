/*
 * measurement_physical_quantities.c
 *
 *  Created on: 3 лют. 2025 р.
 *      Author: centr
 */

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <sys/_intsup.h>
#include <sys/types.h>
#include <unistd.h>
#include "driver/gpio.h"
#include "rom/gpio.h"
#include "config_GPIO.h"
#include "measurement_physical_quantities.h"
#include "DS18B20.h"
#include "DHT.h"
#include "WS2812D.h"
#include "BMP180.h"
#include "LSM303DLHC.h"
#include "statistics.h"

#define cicle 10
uint8_t flag = 255;
uint8_t test = 0;
uint8_t DS18B20_err_convert = 0;

void print_all_param (void){
		printf("\r\n");
		print_PowerOnCount();
		print_time_PowerOnHours();
		print_Uptime();
		DS18B20_print_T ();
		DHT11_print_Rh_T ();
		BMP180_print_P_T ();
		LSM303_print();
}


void state_machine_measurement (void){
	extern uint8_t tic;
	//============ TEST ===============
	if ((tic == 3) && (flag != tic)) {
		print_all_param ();
		flag = tic;
	}
	//============ TEST ===============
	
	if ((tic == 0) && (flag != tic)) {
		//----------- Power on DS18B20, DHT11 -------
		gpio_set_level(POW_DS_DH, 1);
		//- BMP180 read P, T  -
		getPressure_BMP180 ();
		//- LSM303DLHC read acell., magn.
		get_LSM303 ();
		//-----------
		flag = tic;
	}
	
	if ((tic == 1) && (flag != tic)) {
		//- DHT11 read Rh., T.
		get_T_Rh_DHT11 ();
		//- DS18B20 configuration and start conversion temperature -
		if(TEST_AND_WRITE_DEFAULT_SRAM()==-1){
			DS18B20_err_convert = 1;
		}
		else{
			vTaskDelay(10/portTICK_PERIOD_MS);	//1ms;
			if(start_t_convert_DS12B20()==-1){
				DS18B20_err_convert = 1;
			}
		}

		//-------
		flag = tic;
	}
		
	if ((tic == 2) && (flag != tic)) {
		//---- read T - DS18B20 --------------
		if(DS18B20_err_convert == 0){
			READ_TEMPERATURE();	
		}
		//----------- Power off DS18B20, DHT11 -------
		vTaskDelay(10/portTICK_PERIOD_MS);	//1ms;
		gpio_set_level(POW_DS_DH, 0);
		flag = tic;
	}
	
	if (tic >= cicle) {
		//----- reset state machine ---------------
		tic = 0; 
		flag = 255;
		DS18B20_err_convert = 0;
	}

}
