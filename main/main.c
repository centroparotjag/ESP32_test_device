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

// loop task
void loop_task(void *pvParameter)
{
	while(1) {
		gpio_set_level(LED_D2, gpio_get_level(DQ_DH));
		gpio_set_level(LED_D2, gpio_get_level(DQ_DS));
		vTaskDelay(200/portTICK_PERIOD_MS);
		
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
	xTaskCreate(&loop_task, "loop_task", 2048, NULL, 5, NULL);

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

    }
}



