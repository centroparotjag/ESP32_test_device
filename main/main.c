#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <hal/gpio_types.h>
#include "driver/gpio.h"
#include "rom/gpio.h"
#include "config_GPIO.h"




void app_main(void)
{
	init_GPIO ();
	
	int i = 0;
	int state = 0;
	
    while (true) {
        printf("Inial init project! %d\n", i++);
		gpio_set_level(LED_D2, state);
		state = !state;
        sleep(1);
    }
}



