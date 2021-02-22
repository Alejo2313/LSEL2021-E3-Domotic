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
/******************************** Defines **********************************/



/******************************** Types   **********************************/



/******************************** Variables ********************************/



/******************************** Prototypes *******************************/

static char* TAG = "MAIN:"; //<! Trace header

/******************************** Functions ********************************/

/**
 * @brief Main app
 * 
 */
void app_main(void)
{
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_TCP", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_SSL", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

    ESP_ERROR_CHECK(nvs_flash_init());
 
    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    wifi_init_sta();

    mqtt_start();

    while(!IS_FLAG(&flags, MQTT_CONNECTED | WIFI_CONNECTED))
    {
        printf("WAIT \n");
        vTaskDelay(50);
    }

    int state;
    state = topic_subscribe("/test/qos1");
    printf("Topic id %d", state);
    state = topic_subscribe("/test/qos2");
    printf("Topic id %d", state);

    ESP_LOGI(TAG, "LOOP!");

    char* data;
    uint16_t size;
    int index;

    while(1)
    {
    
        if(IS_FLAG(&flags,MQTT_SUBSCRIBED))
        {
            CLEAR_FLAGS(&flags, MQTT_SUBSCRIBED);
        }

        if(IS_FLAG(&flags, MQTT_NEWDATA))
        {
            index = getIncomeData(&data, &size);
            printf("Index %d, data %s",index, data );
            fflush(stdout);
            CLEAR_FLAGS(&flags, MQTT_NEWDATA);
        }
        vTaskDelay(100);
    }
}
