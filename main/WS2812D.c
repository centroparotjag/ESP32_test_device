/*
 * WS2812D.c
 *
 *  Created on: 25 січ. 2025 р.
 *      Author: centr
 */

#include "WS2812D.h"
#include "config_GPIO.h"
#include <driver/gpio.h>
#include <hal/gpio_types.h>
#include <rom/gpio.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/_intsup.h>
#include <sys/types.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#define data(state) 

void reset (void){
	gpio_set_level(DATA_2LED, 0);
	usleep (300);
}

void tx_bit (int b){
	if(b==0){
		gpio_set_level(DATA_2LED, 1);		// 220-380ns
	}
	else {
		for (int i=0; i<3; i++) {
			gpio_set_level(DATA_2LED, 1);	// 580ns-1us
		}
	}
	
	gpio_set_level(DATA_2LED, 0);
}

void convert_byte_to_1barray (uint8_t byte, uint8_t* s_arr){
	int8_t maska = 0x01;

	for (int i=8; i>0; i--) {		
		s_arr[i-1] = (byte & maska) ? 1 : 0;
		maska <<=1;					// maska shift
	} 
}

void convert_RGB_to_1barray (uint8_t R, uint8_t G, uint8_t B, uint8_t* arr){
	uint8_t Rarr[8], Garr[8], Barr[8];
	
	convert_byte_to_1barray (R, Rarr);
	convert_byte_to_1barray (G, Garr);
	convert_byte_to_1barray (B, Barr);
	
	for (int i=0; i<8; i++) {
		arr[i]=Rarr[i];
	}
	
	for (int i=8; i<16; i++) {
		arr[i]=Garr[i-8];
	}
	
	for (int i=16; i<24; i++) {
		arr[i]=Barr[i-16];
	}
}

void led_data (uint8_t R, uint8_t G, uint8_t B){
	uint8_t arr[24];
		
	convert_RGB_to_1barray ( R, G, B, arr);
	
	reset ();
	for (int i=0; i<24; i++) {
		tx_bit (arr[i]);
	}
	for (int i=0; i<24; i++) {
		tx_bit (arr[i]);
	}
}


uint8_t R1, G1, B1, R2, G2, B2;

void led_all_RGB (void){
	uint8_t arr1[24], arr2[24];
		
	convert_RGB_to_1barray ( R1, G1, B1, arr1);
	convert_RGB_to_1barray ( R2, G2, B2, arr2);
	
	reset ();
	for (int i=0; i<24; i++) {
		tx_bit (arr1[i]);
	}
	for (int i=0; i<24; i++) {
		tx_bit (arr2[i]);
	}
	
}

void LED_1_RGB (uint8_t R, uint8_t G, uint8_t B){
	R1=R;
	G1=G;
	B1=B;
	led_all_RGB ();
}

void LED_2_RGB (uint8_t R, uint8_t G, uint8_t B){
	R2=R;
	G2=G;
	B2=B;
	led_all_RGB ();
}

