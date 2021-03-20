/**                             _____________
 *              /\      /\     /             \
 *             //\\____//\\   |   MAUUUU!!    |
 *            /     '      \   \  ___________/
 *           /   /\ '  /\    \ /_/			      / /  ___    / __\ |__   __ _| |_ 
 *          |    == o ==     |        /|	     / /  / _ \  / /  | '_ \ / _` | __|
 *           \      '        /       | |	    / /__|  __/ / /___| | | | (_| | |_ 
 *             \           /         \ \	    \____/\___| \____/|_| |_|\__,_|\__|
 *             /----<o>----\         / /
 *             |            ' \       \ \
 *             |    |    | '   '\      \ \
 *  _________  | ´´ |  ' |     '  \    / /
 *  |  MAYA  | |  ' |    | '       |__/ /
 *   \______/   \__/ \__/ \_______/____/
 * 
 * @file main.c
 * @author Alejandro Gómez (Alejo2313)
 * @author Jaime ....
 * @brief Main file (SOURCE)
 * @version 0.1
 * @date 22/02/2021
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/******************************** Includes *********************************/

#include "mqtt_ext.h"
#include "wifi_conn.h"
#include "defines.h"
#include "platform.h"
/******************************** Defines **********************************/



/******************************** Types   **********************************/



/******************************** Variables ********************************/



/******************************** Prototypes *******************************/

static char* TAG = "MAIN:"; //<! Trace header

/******************************** Functions ********************************/




void app_main(void)
{
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version()); 

//    configPWM();
    sensorInit();
//    uint8_t duty = 30;
    while (1)
    {
        printf("temp %d, hum %d\n", readTemp(),readHum());
  /*      setPWM(LEDR_GPIO, duty);
        setPWM(LEDG_GPIO, duty);
        setPWM(LEDB_GPIO, duty);*/

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    
}