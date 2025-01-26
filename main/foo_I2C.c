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

// Error library
#include "esp_err.h"


#define SDA_PIN GPIO_NUM_21
#define SCL_PIN GPIO_NUM_22

#define I2C_MASTER_ACK  0
#define I2C_MASTER_NACK 1


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
	i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 100, 100, 0);
}



int8_t FM24CL_wr_byte (uint8_t dev_addr, uint8_t reg_addr, uint8_t* reg_data, uint8_t cnt)
{
	int8_t iError;
	esp_err_t espRc;
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();

/*	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_WRITE, true);

	i2c_master_write_byte(cmd, reg_addr, true);
	i2c_master_write(cmd, reg_data, cnt, true);
	i2c_master_stop(cmd);*/


	i2c_master_start(cmd);
	
	i2c_master_write_byte(cmd,0xA0, true);		// FM24CL04B ADDRES + WR
	
	i2c_master_write_byte(cmd,0x00, true);		// mem address
	
/*	i2c_master_write_byte(cmd,0xb0, true);		// data
	i2c_master_write_byte(cmd,0xb1, true);
	i2c_master_write_byte(cmd,0xb2, true);
	i2c_master_write_byte(cmd,0xb3, true);
	i2c_master_write_byte(cmd,0xb4, true);
	i2c_master_write_byte(cmd,0xb5, true);
	i2c_master_write_byte(cmd,0xb6, true);
	i2c_master_write_byte(cmd,0xb7, true);*/
	
	
	for (int i=0; i<8; i++) {
		i2c_master_write_byte(cmd,0xb6+i, true);
	}
	

	i2c_master_stop(cmd);



	espRc = i2c_master_cmd_begin(I2C_NUM_0, cmd, 10 / portTICK_PERIOD_MS);
	
	if (espRc == ESP_OK)
	{
		iError = 1;
	}
	else
	{
		iError = 0;
	}
	i2c_cmd_link_delete(cmd);

	return iError;
}


int8_t FM24CL_read_byte(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt)
{
	int8_t iError;
	esp_err_t espRc;

	i2c_cmd_handle_t cmd = i2c_cmd_link_create();

/*	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_WRITE, true);
	i2c_master_write_byte(cmd, reg_addr, true);

	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_READ, true);

	if (cnt > 1)
	{
		i2c_master_read(cmd, reg_data, cnt - 1, I2C_MASTER_ACK);
	}
	i2c_master_read_byte(cmd, reg_data + cnt - 1, I2C_MASTER_NACK);
	i2c_master_stop(cmd);*/

	uint8_t data[8] = {0};

	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, 0xA0, true);
	i2c_master_write_byte(cmd, 0x00, true);
	
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, 0xA1, true);
	
/*	for (int i=0; i<7; i++) {
		i2c_master_read(cmd, &data[i], 1, I2C_MASTER_ACK);
	}
	*/
	i2c_master_read(cmd, data, 7, I2C_MASTER_ACK);
	i2c_master_read(cmd, &data[7], 1,I2C_MASTER_NACK);
	
	i2c_master_stop(cmd);


	printf("\n\n");
	for (int i=0; i<8; i++) {
		printf("0x%X, ", data[i]);
	}
	printf("\n\n");
	

	espRc = i2c_master_cmd_begin(I2C_NUM_0, cmd, 10 / portTICK_PERIOD_MS);
	if (espRc == ESP_OK)
	{
		iError = 1;
	}
	else
	{
		iError = 0;
	}

	i2c_cmd_link_delete(cmd);

	return iError;
}


#define I2C_MASTER_RX_BUF_DISABLE 0
#define I2C_MASTER_TX_BUF_DISABLE 0
#define I2C_MASTER_FREQ_HZ 100000


void scan_i2c_addr (void){
	printf("i2c scanner\r\n\r\n");

	// configure the i2c controller 0 in master mode, normal speed
	// https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html
	i2c_config_t conf;
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = 21;
	conf.scl_io_num = 22;
	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
	conf.master.clk_speed = I2C_MASTER_FREQ_HZ; //100000
	conf.clk_flags = 0; //(V4.4)  is 0, the clock allocator will select only according to the desired frequency.
	ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &conf));
	printf("- i2c controller configured\r\n");

	// install the driver
	ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0));
	printf("- i2c driver installed\r\n\r\n");

	printf("scanning the bus...\r\n\r\n");
	int devices_found = 0;

	for(int address = 1; address < 127; address++) {

		// create and execute the command link
		i2c_cmd_handle_t cmd = i2c_cmd_link_create();
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE, true);
		i2c_master_stop(cmd);
		if(i2c_master_cmd_begin(I2C_NUM_0, cmd, 100 / portTICK_PERIOD_MS) == ESP_OK) {  
			printf("-> found device with address 0x%02x\r\n", address);
			devices_found++;
		}
		i2c_cmd_link_delete(cmd);
	}
	if(devices_found == 0) printf("\r\n-> no devices found\r\n");
	printf("\r\n...scan completed!\r\n");

}






