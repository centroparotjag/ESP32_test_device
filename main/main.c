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
#include "BMP180.h"
#include "config_GPIO.h"
#include "foo_I2C.h"
#include "statistics.h"
#include "LSM303DLHC.h"


#define date __DATE__
#define time __TIME__

// loop task
void loop_task(void *pvParameter)
{
	while(1) {
		vTaskDelay(1000/portTICK_PERIOD_MS);
		PowerOnHours_update_set_RAM ();		// add 1s
	}
}


void app_main(void)
{
	float T; 
    uint8_t Rh;
    int Tmp; 
	unsigned int Pr;
    sleep (1);
    printf("Firmware build: %s %s\n", date, time);
    init_GPIO();
    i2c_master_init();
    check_I2C_device();
    PowerOnCount_set_1_count_in_FRAM();
    print_PowerOnCount();
    counting_time_PowerOnHours();
    print_Uptime();
    xTaskCreate(&loop_task, "loop_task", 2048, NULL, 5, NULL);
	config_LSM303 ();

    while (true) {
//		printf("\r\n");
//		print_PowerOnCount();
//		counting_time_PowerOnHours();
//		print_Uptime();
//		
//		
//		LED_1_RGB (50, 0, 0);
//		int status = get_T_Rh_DHT11 (&T, &Rh);
//		LED_1_RGB (0, 0, 0);
//		
//		LED_2_RGB (0, 50, 0);
//		READ_TEMPERATURE();
//		LED_2_RGB (0, 0, 0);
//		
//		gpio_set_level(POW_DS_DH, 0);
//		
//		
//		if(status==1) {
//			printf("DHT11   T = %.1fC, Rh = %d%%\n", T, Rh);
//		}
//		
//
//		getPressure_BMP180 ( &Tmp, &Pr);
//		float p_180 = (float)Pr/100;
//		float t_180 = (float)Tmp/10;
//		printf("BMP180  T = %0.1fC; P = %0.2fhPa; \r\n", t_180, p_180);
		LSM303_print();
		
        sleep (2);


		

    }
}



