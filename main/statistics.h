/*
 * statistics.h
 *
 *  Created on: 27 січ. 2025 р.
 *      Author: centr
 */

#ifndef MAIN_STATISTICS_H_
#define MAIN_STATISTICS_H_

#include <stdint.h>

uint32_t PowerOnHours_GET_FRAM (void);
void PowerOnHours_update_set_RAM (void);
void counting_time_PowerOnHours(void);
uint32_t PowerOnCount_GET_FRAM (void);
void PowerOnCount_set_1_count_in_FRAM(void) ;
void print_PowerOnCount(void);
void init_counter_Uptime (const uint32_t* PowOnH);
void print_Uptime(void);

#endif /* MAIN_STATISTICS_H_ */
