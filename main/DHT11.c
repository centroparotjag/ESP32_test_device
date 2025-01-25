/*
 * DHT11.c
 *
 *  Created on: 24 січ. 2025 р.
 *      Author: centr
 */
#include "DHT.h"
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


void send_start_signal (void){
	gpio_set_level(DQ_DH, 0);
	vTaskDelay(25/portTICK_PERIOD_MS);
	gpio_set_level(DQ_DH, 1);
}

int get_data_bit_DHT11 (void){
	uint8_t i = 0;
	int data = 0;
	
	while( gpio_get_level(DQ_DH) == 0){		// waiting for the low |_| state
		i++;
		usleep (1);
		if(i==0xff){break;}
		
	}		
	i=0;
	while( gpio_get_level(DQ_DH) == 1){ 		// we count the time of high |-| state 
		i++;
		usleep (1);
		if(i==0xff){return -1;}
	}
										
	data = (i<40) ? 0 : 1;		// bit = 0 or 1 ?
	
	return data;
}


int crc_check_DHT11 (const unsigned char* data){
	uint16_t total = 0;
	
	for (int i=0; i<4; i++) {
		total += data[i];
	} 
	
	total &= 0x00FF;
	
	if ((uint8_t) total == data[4]){
		return 1;
	}
	else {
		return -1;
	}
}


int covert_data_DHT11 (const unsigned char* data, float *T, uint8_t *Rh){
	uint8_t data_Byte [5] = {0};
	
	//------ 40bit to 5 Byte --------------
	for (int k = 0; k<5; k++) {
		for (int i = 0; i<8; i++) {
			if(i!=0){ data_Byte [k] <<= 1; }
			data_Byte [k]  |= data[i + (k*8)];
		}
	}
	
	//---------- CRC Check -----------------

	if (crc_check_DHT11 (data_Byte) < 1 ){
		printf ("CRC DHT11 - ERROR!\n");
	}
	
	
	*Rh = data_Byte[0];
	float tmp = data_Byte[3];
	*T  = data_Byte[2] + (tmp/10);	
	
	return 1;
}



int get_T_Rh_DHT11 (float *T, uint8_t *Rh){
	uint8_t data[40] = {0};
	float t;
	uint8_t r;
	uint8_t h = 0;
	uint8_t l = 0;
	
	//----------- Power on --------------------------------------
	gpio_set_level(POW_DS_DH, 1);
	vTaskDelay(500/portTICK_PERIOD_MS);	//100ms

	//----------- Send start signal ------------------------------
	send_start_signal ();

	//----------- Read response signal ---------------------------
	while( gpio_get_level(DQ_DH) == 1){			// |¯|  
		h++;
		usleep (1);
		if(h==0xff){break;}
	}

	while( gpio_get_level(DQ_DH) == 0){			// |_|
		l++;
		usleep (1);
		if(l==0xff){break;}
	}

	//---------- response signal check ----------------------------
	if ( (h>4 && h<40) && (l>40 && l<80) ){
		// response - ok!
	}
	else {
		printf ("DHT11 Response signal - ERROR!\n");
		return -1;
	}

	l = 0;
	while( gpio_get_level(DQ_DH) == 1){ // |¯|
		l++;
		usleep (1);
		if(l==0xff){break;}
	}			  

	//---------- read 40 bit data ---------------------------------
	for (int i = 0; i<40; i++) {
		data[i] = (uint8_t) get_data_bit_DHT11 ();
	}
	
	//----------- Power ff --------------------------------------
	gpio_set_level(POW_DS_DH, 0);
	
	//---------	
	int status = covert_data_DHT11 (&data, &t, &r);

	*T = t;
	*Rh = r;

	//printf("DHT11: T=%.1f, Rh=%d\n", t, r);

	return status;
}
	
	

	
