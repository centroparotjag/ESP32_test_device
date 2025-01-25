#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/unistd.h>
#include <unistd.h>
#include <hal/gpio_types.h>
#include "driver/gpio.h"
#include "rom/gpio.h"
#include "config_GPIO.h"
#include "foo_I2C.h"
#include "DHT.h"
//#include "freertos/FreeRTOS.h"
//#include "freertos/event_groups.h"

void app_main(void)
{
	float T; 
    uint8_t Rh;
	
	
	init_GPIO ();
	//i2c_master_init();
	//gpio_set_level(POW_DS_DH, 1);
	sleep (1);
	
	printf("Startup\n");
	
    while (true) {

		gpio_set_level(LED_D2, 1);
		int status = get_T_Rh_DHT11 (&T, &Rh);
		gpio_set_level(LED_D2, 0);
		
		if(status==1) {
			printf("*DHT11: T=%.1f, Rh=%d\n", T, Rh);
		}
		
        sleep (10);

    }
}



