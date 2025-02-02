/*
 * LSM303DLHC.h
 *
 *  Created on: 1 лют. 2025 р.
 *      Author: centr
 */

#ifndef MAIN_LSM303DLHC_H_
#define MAIN_LSM303DLHC_H_

#include <stdint.h>
void config_LSM303 (void);
void get_LSM303 (int16_t* a_X, int16_t* a_Y, int16_t* a_Z, int16_t* m_X, int16_t* m_Y, int16_t* m_Z);
void LSM303_print(void);
//void read_LSM303_acell  (uint16_t* acell_X, uint16_t* acell_Y, uint16_t* acell_Z);
//void read_LSM303_magn (uint16_t* magn_X,  uint16_t* magn_Y,  uint16_t* magn_Z);
#endif /* MAIN_LSM303DLHC_H_ */
