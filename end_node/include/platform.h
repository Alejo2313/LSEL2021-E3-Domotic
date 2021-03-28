#ifndef __PLATFORM_H__
#define __PLATFORM_H__
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
 * @brief Platform file (HEADER)
 * @version 0.1
 * @date 20/03/2021
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/******************************** Includes *********************************/
#include <stdint.h>

#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "bme280.h"
#include "bme280_defs.h"

#include "fsm.h"
#include "wifi_conn.h"
#include "mqtt_ext.h"
#include "defines.h"
#include "fsm_functions.h"
#include "server.h"




/******************************** Defines **********************************/

//LED Definitions

#define LED_TIMER       LEDC_TIMER_1
#define LED_MODE        LEDC_HIGH_SPEED_MODE

#define LED_FADE        0

#define LEDR            1
#define LEDR_GPIO       0
#define LEDR_CHANNEL    LEDC_CHANNEL_0

#define LEDG            2
#define LEDG_GPIO       2
#define LEDG_CHANNEL    LEDC_CHANNEL_1

#define LEDB            3
#define LEDB_GPIO       4
#define LEDB_CHANNEL    LEDC_CHANNEL_2


//I2C definitions 

#define BME_ADDR        BME280_I2C_ADDR_PRIM
#define SDA_PIN         GPIO_NUM_18
#define SCL_PIN         GPIO_NUM_19
#define I2C_PORT        I2C_NUM_0
#define I2C_SPEED       1000000

#define BTN_PIN         GPIO_NUM_15
#define ALARM_PIN       GPIO_NUM_8

// GPIO definitions

#define GPIO_INPUT_MASK     BIT_SHIFT(BTN_PIN)  //| BIT_SHIFT(another_pin) .....
#define GPIO_OUTPUT_MASK    0                   //| BIT_SHIFT(another_pin) .....





/******************************** Types   **********************************/


/******************************** Prototypes *******************************/

/**
 * @brief Configure PWM Hardware
 * 
 */
void configPWM(); //

/**
 * @brief update PWM duty cycle
 * 
 * @param GPIO  pin to update
 * @param dutyCycle duty cycle
 */
void setPWM (uint32_t GPIO, uint8_t dutyCycle); //

/**
 * @brief Initialize GPIOs 
 * 
 */
void initGPIO(); 

/**
 * @brief Set GPIO to low
 * 
 * @param GPIO pin
 */              
void resetGPIO(uint32_t GPIO);      //

/**
 * @brief Set GPIO to High
 * 
 * @param GPIO 
 */
void setGPIO(uint32_t GPIO);        //

/**
 * @brief Read GPIO value
 * 
 * @param GPIO pin
 * @return int GPIO value 
 */
int readGPIO(uint32_t GPIO);        //

/**
 * @brief Initizalize bme280 driver 
 * 
 */
void sensorInit();  //

/**
 * @brief Read humidity
 * 
 * @return int relative humidity (percentage %)
 */
int readHum();      //

/**
 * @brief Read temperature
 * 
 * @return int temperature (mºC)
 */
int readTemp();     //

/**
 * @brief Read light level
 * 
 * @return int 
 */
int readLight();    //

/**
 * @brief Read pressure
 * 
 * @return int pressure 
 */
int readPress();

/**
 * @brief Enter in configuration mode
 * 
 */
void enterConfigMode(void);    


/**
 * @brief Delay millisencods 
 * 
 * @param ms Milliseconds to wait
 */
void delayMs(uint32_t ms);  

/**
 * @brief Get the Tick Count 
 * 
 * @return uint64_t number of ticks 
 */
uint64_t getTickCount();   



void save_data();
void load_data();

#endif // DEBUG