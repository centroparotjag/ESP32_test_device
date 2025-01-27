/*
 * foo_I2C.c
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
#include <stdint.h>
#include <string.h>
#include <stdio.h>
//#include "sdkconfig.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c_master.h"
#include "driver/i2c.h"
#include "foo_I2C.h"
#include "driver/i2c_master.h"
#include <unistd.h>
// Error library
#include "esp_err.h"
#include "hal/i2c_types.h"

#define SDA_PIN GPIO_NUM_21
#define SCL_PIN GPIO_NUM_22

#define I2C_MASTER_ACK  0
#define I2C_MASTER_NACK 1

#define ADDRESS_FM24CL04B	0xA0>>1 
#define ADDRESS_BM180		0xEE>>1 
#define ADDRESS_LSM303DLHC	0x32>>1 

void i2c_master_init(void)
{
	i2c_config_t i2c_config = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = 21,
		.scl_io_num = 22,
		.sda_pullup_en = GPIO_PULLUP_ENABLE,
		.scl_pullup_en = GPIO_PULLUP_ENABLE,
		.master.clk_speed = 400000
		};
	i2c_param_config(I2C_NUM_0, &i2c_config);
	i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER,0, 0, 0);
}

//============ Checking for device (I2C) availability =============================
int presense_device_I2C (uint8_t addr){
		i2c_cmd_handle_t cmd = i2c_cmd_link_create();
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true);
		i2c_master_stop(cmd);
		if(i2c_master_cmd_begin(I2C_NUM_0, cmd, 100 / portTICK_PERIOD_MS) == ESP_OK) {  
			i2c_cmd_link_delete(cmd);
			return 1;
		}
		return -1;
}

void check_I2C_device (void){
	int check_FM24CL04		= presense_device_I2C (ADDRESS_FM24CL04B);
	int check_BMP180		= presense_device_I2C (ADDRESS_BM180);
	int check_LSM303DLHC	= presense_device_I2C (ADDRESS_LSM303DLHC);
	
	if(check_FM24CL04!=1){
		printf("ERROR - FM24CL04 (I2C address 0x%02X - not detected!!!)\r\n", ADDRESS_FM24CL04B);
	}
	if(check_BMP180!=1){
		printf("ERROR - BMP180 (I2C address 0x%02X - not detected!!!)\r\n", ADDRESS_BM180);
	}
	if(check_LSM303DLHC!=1){
		printf("ERROR - LSM303DLHC (I2C address 0x%02X - not detected!!!)\r\n", ADDRESS_LSM303DLHC);
	}
}

//======================= FM24CL04 ===============================
uint8_t read_byte_FM24CL04 (uint16_t addr){
	uint8_t byte;
	uint8_t address;
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	
	if (addr > 0xFF){
		address = (ADDRESS_FM24CL04B<<1) | 0x02;		// A0 (page) = 1;
		addr &= 0xFF;		
	}
	else {
		address = ADDRESS_FM24CL04B<<1;
	}
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, address, true);
	i2c_master_write_byte(cmd, addr, true);
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, address+1, true);
	i2c_master_read_byte(cmd, &byte, I2C_MASTER_NACK);
	i2c_master_stop(cmd);
	i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);	
	i2c_cmd_link_delete(cmd);
	
	return byte;
}

void write_byte_FM24CL04 (uint16_t addr, uint16_t byte){
	uint8_t address;
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	if (addr > 0xFF){
		address = (ADDRESS_FM24CL04B<<1) | 0x02;		// A0 (page) = 1;
		addr &= 0xFF;		
	}
	else {
		address = ADDRESS_FM24CL04B<<1;
	}
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, address, true);
	i2c_master_write_byte(cmd, addr, true);
	i2c_master_write_byte(cmd, byte, true);
	i2c_master_stop(cmd);
	i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);
}

void write_data_FM24CL04 (uint16_t addr, uint8_t *data, uint16_t len){
	uint8_t address;
	uint8_t address_ee;
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	
	if (addr > 0xFF){
		address = (ADDRESS_FM24CL04B<<1) | 0x02;		// A0 (page) = 1;	
	}
	else {
		address = ADDRESS_FM24CL04B<<1;
	}
	address_ee = addr & 0xFF;	
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, address, true);
	i2c_master_write_byte(cmd, address_ee, true);
	i2c_master_write(cmd, data, len, true);
	i2c_master_stop(cmd);
	i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);
}

void read_data_FM24CL04 (uint16_t addr, uint8_t *data, uint16_t len){
	uint8_t address;
	uint8_t address_ee;
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	
	if (addr > 0xFF){
		address = (ADDRESS_FM24CL04B<<1) | 0x02;		// A0 (page) = 1;
	}
	else {
		address = ADDRESS_FM24CL04B<<1;
	}
	address_ee = addr & 0xFF;
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, address, true);
	i2c_master_write_byte(cmd, address_ee, true);
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, address+1, true);
	i2c_master_read(cmd, data, len, I2C_MASTER_ACK);
	i2c_master_stop(cmd);
	i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);	
	i2c_cmd_link_delete(cmd);
}




