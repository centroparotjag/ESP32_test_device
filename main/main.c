#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/unistd.h>
#include <unistd.h>
#include <hal/gpio_types.h>
#include "driver/gpio.h"
#include "rom/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "DS18B20.h"
#include "DHT.h"
#include "WS2812D.h"
#include "config_GPIO.h"
#include "foo_I2C.h"
#include "statistics.h"

// loop task
void loop_task(void *pvParameter)
{
	while(1) {
		vTaskDelay(1000/portTICK_PERIOD_MS);
		PowerOnHours_update_set_RAM ();
		

	}
}


void app_main(void)
{
	float T; 
    uint8_t Rh;
    sleep (1);
	printf("Startup\n");
	init_GPIO ();
	i2c_master_init();
	check_I2C_device ();
	PowerOnCount_set_1_count_in_FRAM();
	print_PowerOnCount();
	counting_time_PowerOnHours();
	print_Uptime();
	xTaskCreate(&loop_task, "loop_task", 2048, NULL, 5, NULL);



	
//	uint8_t data[4] = {0xFF, 0xFF, 0xFF, 0x00};
//	write_data_FM24CL04(0x0010, &data, 4);


    while (true) {

		LED_1_RGB (50, 0, 0);
		int status = get_T_Rh_DHT11 (&T, &Rh);
		LED_1_RGB (0, 0, 0);
		
		LED_2_RGB (0, 50, 0);
		READ_TEMPERATURE();
		LED_2_RGB (0, 0, 0);
		
		gpio_set_level(POW_DS_DH, 0);
		if(status==1) {
			printf("DHT11   T = %.1fC, Rh = %d%%\n", T, Rh);
		}
		
        sleep (10);
        
        printf("\r\nStaistics \r\n");
        print_PowerOnCount();
		counting_time_PowerOnHours();
		print_Uptime();
		printf("\r\n");

    }
}



