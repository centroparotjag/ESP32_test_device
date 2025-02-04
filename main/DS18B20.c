/*
 * DS18B20.c
 *
 *  Created on: 26 січ. 2025 р.
 *      Author: centr
 */
#include "DS18B20.h"
#include "config_GPIO.h"
#include <driver/gpio.h>
#include <hal/gpio_types.h>
#include <math.h>
#include <rom/gpio.h>
#include "config_GPIO.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include <stdint.h>
#include <unistd.h>

extern float T_ds18b20;

//---------------------- PRESENCE -----------------------------
int DS18B20_RESET_PRESENCE(void)
{
	unsigned char STATUS_PRESENCE;
	gpio_set_level(DQ_DS, 1);
	vTaskDelay(10/portTICK_PERIOD_MS);	// 1ms;
	gpio_set_level(DQ_DS, 0);			// 0
	usleep (600);								// 600u
	gpio_set_level(DQ_DS, 1);			// 1
	usleep (60);								// 60u
	STATUS_PRESENCE = gpio_get_level(DQ_DS);
	if(STATUS_PRESENCE == 1){
		printf("ERROR - DS18B20 not presence!!!");
		return -1;
	}
	usleep (600);								// 600u
	return 1;
}

//----------------- MASTER WRITE SLOT ------------------------
void WRITE_SLOT(unsigned char SLOT) {
	gpio_set_level(DQ_DS, 0);	//1

	if (SLOT)	//////////// 1 ///////////   --|_|---------
	{
		usleep (2);			// 5u
		gpio_set_level(DQ_DS, 1);	//1
		usleep (62); 			// 62u
	}
	else		//////////// 0 ///////////	--|_________|--
	{
		usleep (65); 						// 65u
		gpio_set_level(DQ_DS, 1);	// 1
		usleep (5);						// 5u
	}
	usleep (5);							// 5u
}

//---------- MASTER READ SLOT ---------------------------------------
unsigned char READ_SLOT(void) {
	unsigned char READ = 0;
	gpio_set_level(DQ_DS, 0);	// 0
	usleep (5);						// 5u
	gpio_set_level(DQ_DS, 1);	// 1
	usleep (5);						// 5u
	READ = gpio_get_level(DQ_DS);
	usleep (70);						// 70u
	return READ;
}

//----------- MASTER READ BYTE (Little endian) ----------------------
unsigned char READ_BYTE(void) {
	unsigned char BYTE = 0;
	for (unsigned char i = 0; i < 8; ++i)
	{
		BYTE |= ((READ_SLOT()) << i);
	}
	return BYTE;
}

//----------- MASTER WRITE BYTE (Little endian) ----------------------
void WRITE_BYTE(unsigned char DATA) {
	unsigned char arr[9] = { 0 };

	for (uint8_t i = 0; i < 8; ++i)
	{
		arr[i] = (DATA >> i) & 0x01;
	}

	for (uint8_t i = 0; i < 8; ++i)
	{
		WRITE_SLOT(arr[i]);
	}
}

//----------------------- CRC --------------------------------------------------
unsigned char dsCRC8(const unsigned char* addr, unsigned char len) {			//CRC 1-WIRE DS18B20

	unsigned char crc = 0;
	while (len--)
	{
		unsigned char inbyte = *addr++;
		for (unsigned char i = 8; i; i--)
		{
			unsigned char mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix) crc ^= 0x8C;
			inbyte >>= 1;
		}
	}
	return crc;
}

//---------------------- READ SCRATCHPAD --------------------------------------
int SCRATCHPAD_READ(unsigned char* skratchpad) {

	unsigned char i, SCRATCHPAD_i[9];

	WRITE_BYTE(SKIP_ROM);					// DQ, DATA
	WRITE_BYTE(READ_SCRATCHPAD);			// DQ, DATA

	//-------- READ 9 BYTE SCRATCHPAD --------
	for (i = 0; i < 9; ++i) {
		SCRATCHPAD_i[i] = READ_BYTE();	// DQ
	}

	//---- comparison of the CRC --------
	if (SCRATCHPAD_i[8] == dsCRC8(SCRATCHPAD_i, 8))
	{
		for (i = 0; i < 9; ++i) {
			skratchpad[i] = SCRATCHPAD_i[i];	// Copy SCRATCHPAD to in external buffer
		}
		return 1;								// CRC - OK (0x01)
	}
	else
	{
		for (i = 0; i < 9; ++i) {
			skratchpad[i] = 0;					// RESET DATA in external SCRATCHPAD buffer
		}
		printf("CRC DS18B20 - ERROR!");
		return -1;								// CRC - ERROR (0x00)
	}
}

//----------------------- TEST AND WRITE DEFAULT STATE SRAM -------------
int TEST_AND_WRITE_DEFAULT_SRAM(void) {
	unsigned char scratchpad[9];
	unsigned char i, CONFIG_RESOLUTION;
	CONFIG_RESOLUTION = RESOLUTION_12b;			// SET DEFAULT RESOLUTION
	if (DS18B20_RESET_PRESENCE()==-1) {
		printf("ERROR - DS18B20 not presence2!!!");
		return -1;							// error
	}


	//--------------- READ SCRATCHPAD ----------------------------
	i = 0;
	while (SCRATCHPAD_READ(scratchpad) == 0 && i < 10) { i++; }

	//-------------- check resolution in SRAM --------------------
	if (scratchpad[4] == CONFIG_RESOLUTION) {	// Config Register
		return OK;
	}
	else {}

	//================= NEW SEQUENCE WRITE SCRATCHPAD ===================
	if (DS18B20_RESET_PRESENCE()==-1) {
		printf("ERROR - DS18B20 not presence3!!!");
		return -1;							// error
	}

	WRITE_BYTE(SKIP_ROM);					// DQ, DATA
	WRITE_BYTE(WRITE_SCRATCHPAD);			// DQ, DATA

	WRITE_BYTE(0x4B);						// DQ, Th (default manufacture). SRAM 0x02
	WRITE_BYTE(0x46);						// DQ, Tl (default manufacture). SRAM 0x03
	WRITE_BYTE(CONFIG_RESOLUTION);			// DQ, config. 				     SRAM 0x04

	if (DS18B20_RESET_PRESENCE()==-1) {
		printf("ERROR - DS18B20 not presence4!!!");
		return -1;							// error
	}

	if (!SCRATCHPAD_READ(scratchpad)) {				//READ SCRATCHPAD and CRC CHECK
		return 255;
	}
	else {}

	//-------------- check resolution in SRAM --------------------
	if (scratchpad[4] != CONFIG_RESOLUTION) {	// Config. Register
		return 254;
	}
	else {}

	//================== SAVE ROM in SRAM ==============================
	if (DS18B20_RESET_PRESENCE()==-1) {
		printf("ERROR - DS18B20 not presence5!!!");
		return -1;							// error
	}
	
	WRITE_BYTE(SKIP_ROM);					// DQ, DATA
	WRITE_BYTE(COPY_SCRATCHPAD);			// DQ, DATA

	usleep (1);

	return 1;							   // OK write SRAM
}

//*********************** READ ROM 64b *********************************************

//unsigned char ROM [9] = {};
//
//unsigned char READ_ROM_64 (void){
	//unsigned char i, ROM_i [9];
//
	//if (DS18B20_RESET_PRESENCE ()) {
		//return ERROR;						// error
	//}
	//else {}
//
	//WRITE_BYTE (READ_ROM);					// DQ, DATA
//
	//-------- READ 9 BYTE ROM --------
	//for (i = 0; i < 8; ++i) {
		//ROM_i [i] = READ_BYTE (  );			// DQ
	//}
//
	//---- comparison of the CRC --------
	//if (ROM_i[7] == dsCRC8(ROM_i, 7))
	//{
		//for (i = 0; i < 8; ++i) {
			//ROM [i] = ROM_i [i];			// Copy ROM to in external buffer
		//}
		//return OK;							// CRC - OK (0x01)
	//}
	//else
	//{
		//for (i = 0; i < 9; ++i) {
			//ROM [i] = 0;					// RESET DATA in external SCRATCHPAD buffer
		//}
		//return ERROR;						// CRC - ERROR (0x00)
	//}
//}

//*********************** READ TEMPERATURE ******************************************

unsigned char FLAG_convert = 0;
unsigned char internal_T = 0;
unsigned int T_src;

void time_out_convert_t(unsigned char  timer, unsigned char  conversial_period) {
	 

	if (!(timer % conversial_period) && FLAG_convert == 0)
	{
		TEST_AND_WRITE_DEFAULT_SRAM();
		vTaskDelay(1/portTICK_PERIOD_MS);	//1ms;
		start_t_convert_DS12B20();
		internal_T = timer + 1;
		FLAG_convert = 1;
	}

	if (timer == internal_T)
	{
		T_src = read_t_after_convert_DS12B20();
		FLAG_convert = 0;
		internal_T = 0;
	}
}


int start_t_convert_DS12B20(void) {
	//--------- CONVERT T -----------------------------------
	if (DS18B20_RESET_PRESENCE()==-1) {
		printf("ERROR - DS18B20 not presence6!!!");
		return -1;							// error
	}

	WRITE_BYTE(SKIP_ROM);				// DQ, DATA  0xCC
	WRITE_BYTE(CONVERT_T);				// DQ, DATA 0x44
	return 1;
}

int read_t_after_convert_DS12B20(void) {
	unsigned char scratchpad[9]; 
	//---------- READ T ------------------------------------
	if (DS18B20_RESET_PRESENCE()==-1) {
		printf("ERROR - DS18B20 not presence7!!!");
		return -1;							// error
	}

	return SCRATCHPAD_READ(scratchpad);
}


void t_ds18b20 (float* t, uint8_t h_b, uint8_t l_b){
	uint16_t t_src = (h_b<<8) + l_b;
	if(t_src >= 0xFC90 ){			 // - negative T
		T_ds18b20 = (0xffff - t_src + 1)*0.0625*(-1);
	}
	else {							 // + positive T
		T_ds18b20 = t_src*0.0625;
	}
	
	*t  = T_ds18b20;
}




 int READ_TEMPERATURE(void) {
	uint8_t scratchpad[9];
	float T;
	//--------- CONVERT T -----------------------------------
//	if (DS18B20_RESET_PRESENCE()) {
//		return ERROR;									// error
//	}
//	else {}
//
//	WRITE_BYTE(SKIP_ROM);							// DQ, DATA
//	WRITE_BYTE(CONVERT_T);						// DQ, DATA
//	vTaskDelay(751/portTICK_PERIOD_MS);	// 751ms;

	//---------- READ T ------------------------------------
	if (DS18B20_RESET_PRESENCE()==-1) {
		printf("ERROR - DS18B20 not presence8!!!");
		return -1;							// error
	}

	int status = SCRATCHPAD_READ(scratchpad);
	if(status ==-1) {
		printf("ERROR - DS18B20 SCRATCHPAD_READ2!!!");
		return -1;
	}

	t_ds18b20 (&T, scratchpad[1], scratchpad[0]);
	 
	T_ds18b20 = T;  
	
	//printf ("DS18B20 T = %.1fC\n", T);
	
	return status;
}

void DS18B20_print_T (void){
	extern float T_ds18b20;
	printf ("DS18B20 T = %.1fC\n", T_ds18b20);
}


