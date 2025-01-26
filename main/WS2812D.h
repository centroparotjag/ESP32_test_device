/*
 * WS2812D.h
 *
 *  Created on: 25 січ. 2025 р.
 *      Author: centr
 */
#include <stdint.h>

#ifndef MAIN_WS2812D_H_
#define MAIN_WS2812D_H_

void reset (void);
void convert_byte_to_1barray (uint8_t byte, uint8_t* arr);
void tx_bit (int b);
void led_data (uint8_t R, uint8_t G, uint8_t B);
void led_all_RGB (void);
void LED_1_RGB (uint8_t R, uint8_t G, uint8_t B);
void LED_2_RGB (uint8_t R, uint8_t G, uint8_t B);

#endif /* MAIN_WS2812D_H_ */
