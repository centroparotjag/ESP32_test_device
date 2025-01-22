/*
 * config_GPIO.h
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

#ifndef MAIN_CONFIG_GPIO_H_
#define MAIN_CONFIG_GPIO_H_

	#define POW_DS_DH	17	// +3V3 power DS18B20 and DHT11
	#define DATA_2LED	16	// DATA, 2 address LED WS2812D-F5 
	#define DQ_DS		4	// DQ DS18B20 
	#define LED_D2		2	// led is connected to D2
	#define DQ_DH		15	// DATA DHT11 
	
	void init_GPIO (void);

#endif /* MAIN_CONFIG_GPIO_H_ */
