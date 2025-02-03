#include "main.h"

// loop task
void loop_task(void *pvParameter)
{
	while(1) {
		vTaskDelay(1000/portTICK_PERIOD_MS);
		PowerOnHours_update_set_RAM ();		// add 1s
		tic++;
	}
}


void app_main(void)
{
    sleep (1);
    printf("\r\nStartup\r\nFirmware build: %s %s\r\n", __DATE__, __TIME__);
    init_GPIO();
    i2c_master_init();
    check_I2C_device();
    PowerOnCount_set_1_count_in_FRAM();
    xTaskCreate(&loop_task, "loop_task", 2048, NULL, 5, NULL);
	config_LSM303 ();

    while (true) {
		state_machine_measurement ();
		vTaskDelay(100/portTICK_PERIOD_MS);	//100ms;
    }
}



