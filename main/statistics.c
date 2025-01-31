/*
 * statistics.c
 *
 *  Created on: 27 січ. 2025 р.
 *      Author: centr
 */

#include "statistics.h"
#include "foo_I2C.h"
#include <stdint.h>
#include <stdio.h>

#define PowerOnHours 0x0010
#define PowerOnCount 0x0014

uint32_t start_time = 0;
//============= Power On Hours ================
uint32_t PowerOnHours_GET_FRAM (void){
	uint8_t data[4];
	uint32_t s = 0;
	read_data_FM24CL04 (PowerOnHours, data,  4);
	s |= data[0]<<24 | data[1]<<16 | data[2]<<8 | data[3];  // Power On Hours in seconds
	return s;
}

void PowerOnHours_update_set_RAM (void){
	uint8_t data[4] = {0};
	uint32_t s = PowerOnHours_GET_FRAM ();
	s++;									// add 1 second
	data[0] = (s& 0xFF000000) >> 24;
	data[1] = (s& 0x00FF0000) >> 16;
	data[2] = (s& 0x0000FF00) >> 8;
	data[3] = (s& 0x000000FF);
	write_data_FM24CL04(PowerOnHours, data, 4);
}

void counting_time_PowerOnHours(void){
	uint32_t s = PowerOnHours_GET_FRAM ();
	uint32_t H, m, sec;
	
	H = s/3600;
	s -= H * 3600;
	m = s/60;
	s -= m * 60;
	sec = s;
	
	printf("Power On Hours: %0luh:%02dm:%02ds\r\n", (long)H, (uint8_t)m, (uint8_t)sec);
}

//================= Uptime ====================
void init_counter_Uptime (const uint32_t* PowOnH){
	if(start_time == 0){
		start_time = *PowOnH;
	}
}

void print_Uptime(void){
	uint32_t s = PowerOnHours_GET_FRAM ()-start_time;
	uint32_t H, m, sec;
	H = s/3600;
	s -= H * 3600;
	m = s/60;
	s -= m * 60;
	sec = s;
	printf("Uptime: %0luh:%02dm:%02ds\r\n", (long)H, (uint8_t)m, (uint8_t)sec);
}

//============= Power On Count ================
uint32_t PowerOnCount_GET_FRAM (void){
	uint8_t data[4];
	uint32_t c = 0;
	read_data_FM24CL04 (PowerOnCount, data,  4);
	c |= data[0]<<24 | data[1]<<16 | data[2]<<8 | data[3];  // Power On Hours in seconds
	return c;
}

void PowerOnCount_set_1_count_in_FRAM(void) {
	uint8_t data[4] = {0};
	uint32_t c = PowerOnCount_GET_FRAM ();
	c++;									// add 1 count
	data[0] = (c& 0xFF000000) >> 24;
	data[1] = (c& 0x00FF0000) >> 16;
	data[2] = (c& 0x0000FF00) >> 8;
	data[3] = (c& 0x000000FF);
	write_data_FM24CL04(PowerOnCount, data, 4);
	
	//------ Get "zero" time. for counter Uptime -------
	uint32_t s = PowerOnHours_GET_FRAM ();
	init_counter_Uptime (&s);
}

void print_PowerOnCount(void){
	uint32_t c = PowerOnCount_GET_FRAM ();
	printf("Power On Count: %lu\r\n", (long)c);
}








