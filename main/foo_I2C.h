/*
 * foo_I2C.h
 *
 *  Created on: 23 січ. 2025 р.
 *      Author: centr
 */
//-----------------------------//
// i2c device | address		   //
// FM24CL04B	0xA0 (0x50<<1) //
// BM180		0xEE (0x77<<1) //
// LSM303DLHC	0x32 (0x19<<1) //
//-----------------------------//

#ifndef MAIN_FOO_I2C_H_
#define MAIN_FOO_I2C_H_

#include <stdint.h>

void i2c_master_init(void);
void check_I2C_device (void);
uint8_t read_byte_FM24CL04 (uint16_t addr);
void write_byte_FM24CL04 (uint16_t addr, uint16_t byte);
void write_data_FM24CL04 (uint16_t addr, uint8_t *data, uint16_t len);
void read_data_FM24CL04 (uint16_t addr, uint8_t *data, uint16_t len);
void read_data_BMP180_LSM303 (uint8_t addr, uint8_t reg, uint8_t *data, uint8_t len);
void write_byte_BMP180_LSM303 (uint8_t addr, uint8_t reg, uint8_t byte);
void write_data_BMP180_LSM303 (uint8_t addr, uint8_t reg, uint8_t *data, uint16_t len);
uint8_t read_byte_BMP180_LSM303 (uint16_t addr, uint8_t reg);
#endif /* MAIN_FOO_I2C_H_ */
