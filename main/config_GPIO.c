/*
 * config_GPIO.c
 *
 *  Created on: 22 січ. 2025 р.
 *      Author: centr
 */

//////////////////////////////////////////////////////////////////////
//                                                                  //
//          ESP32-WROOM (4MB)                                       //
//          ----------------                                        //
//         |EN           D23|                                       //
//         |VP       SCL/D22| --> I2C FM24CL04B/BMP180/LSM303DLHC   //
//         |VN           TXD|                                       //
//         |D34          RXD|                                       //
//         |D35      SDA/D21| --> I2C FM24CL04B/BMP180/LSM303DLHC   //
//         |D32          D19|                                       //
//         |D33          D18|                                       //
//         |D25           D5|                                       //
//         |D26          D17| --> +3V3 power DS18B20 and DHT11      //
//         |D27          D16| --> DATA, 2 address LED WS2812D-F5    //
//         |D14           D4| --> DQ DS18B20                        //
//         |D12       LED/D2|                                       //
//         |D13          D15| --> DATA DHT11                        //
//         |GND          GND| --> GND                               //
// +5V <-- |VIN          3V3| --> +3V3                              //
//          ----------------                                        //
//                                                                  //
//////////////////////////////////////////////////////////////////////

#include <driver/gpio.h>
#include <hal/gpio_types.h>
#include <rom/gpio.h>
#include "config_GPIO.h"

void init_GPIO (void){
	gpio_pad_select_gpio(POW_DS_DH);
	gpio_set_direction(POW_DS_DH, GPIO_MODE_OUTPUT);
	gpio_set_level(POW_DS_DH, 0);
	
	gpio_pad_select_gpio(DATA_2LED);
	gpio_set_direction(DATA_2LED, GPIO_MODE_OUTPUT);
	gpio_set_level(DATA_2LED, 0);
		
	gpio_pad_select_gpio(DQ_DS);
	gpio_set_direction(DQ_DS, GPIO_MODE_INPUT_OUTPUT_OD);
	gpio_set_level(DQ_DS, 1);
	
	gpio_pad_select_gpio(LED_D2);
	gpio_set_direction(LED_D2, GPIO_MODE_OUTPUT);
	gpio_set_level(LED_D2, 0);
	
	gpio_pad_select_gpio(DQ_DH);
	gpio_set_direction(DQ_DH, GPIO_MODE_INPUT_OUTPUT_OD);
	gpio_set_level(DQ_DH, 1);
}


