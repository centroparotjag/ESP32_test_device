/*
 * DS18B20.h
 *
 *  Created on: 26 січ. 2025 р.
 *      Author: centr
 */

#ifndef MAIN_DS18B20_H_
#define MAIN_DS18B20_H_

#include <stdint.h>

#define SEARCH_ROM			0xF0
#define READ_ROM			0x33
#define MATCH_ROM			0x55
#define SKIP_ROM			0xCC
#define ALARM_SEARCH		0xEC

#define CONVERT_T			0x44
#define WRITE_SCRATCHPAD	0x4E
#define READ_SCRATCHPAD		0xBE
#define COPY_SCRATCHPAD		0x48
#define RECALL_E2			0xB8
#define READ_POWER_SUPPLY	0xB4

#define PROCESS				0x03
#define OK					0x01
#define ERROR				0x00

#define RESOLUTION_9b		0x1F
#define RESOLUTION_10b		0x3F
#define RESOLUTION_11b		0x5f
#define RESOLUTION_12b		0x7F

void DQ_O_I_mode(unsigned char OI);
unsigned char DQ_read(void);
void DQ_OUT(unsigned char Out);
void Power_DS18B20(unsigned char pow_ON);
unsigned char polling_DS18B20(void);
int DS18B20_RESET_PRESENCE(void);
void WRITE_SLOT(unsigned char SLOT);
unsigned char READ_SLOT(void);
unsigned char READ_BYTE(void);
void WRITE_BYTE(unsigned char DATA);
unsigned char dsCRC8(const unsigned char* addr, unsigned char len);
int SCRATCHPAD_READ(unsigned char* skratchpad);
int TEST_AND_WRITE_DEFAULT_SRAM(void);
int READ_TEMPERATURE(void);
unsigned char READ_ROM_64(void);
int read_t_after_convert_DS12B20(void);
int start_t_convert_DS12B20(void);
void time_out_convert_t(unsigned char  timer, unsigned char  conversial_period);
void read_t_DS18B20_test(void);
void t_ds18b20 (float* t, uint8_t h_b, uint8_t l_b);
void DS18B20_print_T (void);
#endif /* MAIN_DS18B20_H_ */
