/*
 * foo_I2C.h
 *
 *  Created on: 23 січ. 2025 р.
 *      Author: centr
 */
//--------------------------//
// i2c device | address		//
// FM24CL04B	0xA0 (0x50) //
// BM180		0xEE (0x77) //
// LSM303DLHC	0x32 (0x19) //
//--------------------------//

#ifndef MAIN_FOO_I2C_H_
#define MAIN_FOO_I2C_H_

#include <stdint.h>

void i2c_master_init(void);
int8_t FM24CL_wr_byte (uint8_t dev_addr, uint8_t reg_addr, uint8_t* reg_data, uint8_t cnt);
int8_t FM24CL_read_byte(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt);
void scan_i2c_addr (void);

#define ADDRESS_FM24CL04B	0xA0>>1 
#define ADDRESS_BM180		0xEE>>1 
#define ADDRESS_LSM303DLHC	0x32>>1 

#endif /* MAIN_FOO_I2C_H_ */
