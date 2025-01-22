#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

void app_main(void)
{
	int i = 0;
    while (true) {
        printf("Inial init project! %d\n", i++);
        sleep(1);
    }
}

//////////////////////////////////////////////////////////////////////
//                                                                  //
//          ESP32-WROOM (4MB)                                       //
// 			----------------                                        //
//         |EN           D23|                                       //
//         |VP       SCL/D22| --> I2C FM24CL04B/BMP180/LSM303DLHC   //
//         |VN           TXD|                                       //
//         |D34          RXD|                                       //
//         |D35      SDA/D21| --> I2C FM24CL04B/BMP180/LSM303DLHC   //
//         |D32          D19|                                       //
//         |D33          D18|                                       //
//         |D25           D5|                                       //
//         |D26          D17| --> +3V3 power DS18B20 and DHT11      //
//         |D27          D16| --> DATA, 2 address LED WS2812D-F5    //
//         |D14           D4| --> DQ DS18B20                        //
//         |D12       LED/D2|                                       //
//         |D13          D15| --> DATA DHT11                        //
//         |GND          GND| --> GND                               //
// +5V <-- |VIN          3V3| --> +3V3                              //
//          ----------------                                        //
//                                                                  //
//////////////////////////////////////////////////////////////////////

