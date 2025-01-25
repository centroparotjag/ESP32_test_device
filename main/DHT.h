/*
 * DHT.h
 *
 *  Created on: 24 січ. 2025 р.
 *      Author: centr
 */

#ifndef MAIN_DHT_H_
#define MAIN_DHT_H_

#include <stdint.h>
void send_start_signal (void);
int get_data_bit_DHT11 (void);
int crc_check_DHT11 (const unsigned char* data);
int covert_data_DHT11 (const unsigned char* data, float *T, uint8_t *Rh);
int get_T_Rh_DHT11 (float *T, uint8_t *Rh);

#endif /* MAIN_DHT_H_ */
