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
static const char *TAG = "Main";

char wifi_ssid[32];
char wifi_password[64];
char mqtt_broker_url[64];


fsm_sensor_t fsm_sensor = 
{
    .data = 
    {
        .flags          = &flags,
        .hum            = 0,
        .light          = 0,
        .temp           = 0,
        .tickCounter    = 0
    },
    .interface = 
    {
        .getTickCount   = getTickCount,
        .readHum        = readHum,
        .readLight      = readPress,
        .readTemp       = readTemp
    }
    
};

fsm_led_t fsm_led = 
{
 
    .data = 
    {
        .flags = &flags,

        .alarmPin = ALARM_PIN,
        
        .rPin     = LEDR_GPIO,
        .gPin     = LEDG_GPIO,
        .bPin     = LEDB_GPIO,

        .rColor   = 0xFF,
        .gColor   = 0xFF,
        .bColor   = 0xFF,
    },
    .interface = 
    {
        .pwmSet     = setPWM,
        .resetGPIO  = resetGPIO,
        .setGPIO    = setGPIO,
        .setColors  = setColors,
    }
    
};

fsm_event_t fsm_event = 
{

    .data = 
    {
        .colorLEDData = &(fsm_led.data),
        .flags =&flags,
        .sensorData = &(fsm_sensor.data),
    },
    .interface = 
    {
        .delayMs = delayMs,
        .getData = getIncomeData,
        .sendData = (void*)topic_publish,
    }
};


fsm_control_t fsm_control = 
{

    .data =
    {
        .btnPin         = BTN_PIN,
        .flags          = &flags,
        .tickCounter    = 0
    },
    .interface =
    {
        .enterConfigMode = enterConfigMode,
        .getTickCount    = getTickCount,
        .mqttConnect     = mqtt_start,
        .readGPIO        = readGPIO,
        .wifiConnect     = wifi_init_sta,

        .delay           = delayMs,
        .serverStart     = start_webserver,
        .serverStop      = stop_webserver,
        
        .subscribe       = topic_subscribe
    
    }
};



/******************************** Prototypes *******************************/

//static char* TAG = "MAIN:"; //<! Trace header

/******************************** Functions ********************************/





void app_main(void)
{

    esp_log_level_set("*", ESP_LOG_VERBOSE);



    //Init system
    
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_init();
    esp_netif_create_default_wifi_sta();
    esp_netif_create_default_wifi_ap();

    //Config hardware 
    load_data();
    sensorInit();
    configPWM();
    
    configSPI();

    //init FSM
    fsm_init((fsm_t*)(&fsm_event), eventos_fsm);
    fsm_init((fsm_t*)(&fsm_led), led_fsm);
    fsm_init((fsm_t*)(&fsm_control), control_fsm);
    fsm_init((fsm_t*)(&fsm_sensor), sensor_fsm);
    
    
    while (1)
    {
 
        
        fsm_fire((fsm_t*)(&fsm_control));
        fsm_fire((fsm_t*)(&fsm_event));
        fsm_fire((fsm_t*)(&fsm_led));
        fsm_fire((fsm_t*)(&fsm_sensor));

        ESP_LOGI(TAG,"stateC %d StateE %d stateL %d stateS %d tick %d flags %X\n", fsm_control.fsm.current_state , fsm_event.fsm.current_state, fsm_led.fsm.current_state,fsm_sensor.fsm.current_state ,fsm_control.interface.readGPIO(BTN_PIN), flags);
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
    
}