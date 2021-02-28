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
 * @file defines.h
 * @author Alejandro Gómez (Alejo2313)
 * @author Jaime...
 * @brief Definitions file (HEADER)
 * @version 0.1
 * @date 22/02/2021
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __DEFINES_H__
#define __DEFINES_H__

/******************************** Includes *********************************/

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
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

#include "esp_log.h"

/******************************** Defines **********************************/

#define BIT_SHIFT(n) (1 << (n))
#define CLEAR_FLAGS(ptrFlags, flag) ( *(ptrFlags) &= ~(flag) )
#define SET_FLAGS(ptrFlags, flag)   ( *(ptrFlags) |= (flag) )
#define IS_FLAG(ptrFlags, flag) ( (*(ptrFlags))&(flag) )


#define CONFIG_BROKER_URL    "mqtt://192.168.1.142:1883"
#define SSID     "test"
#define PASSWORD "test"

/******************************** Types   **********************************/

typedef enum 
{
    WIFI_CONNECTED      = BIT_SHIFT(0),
    MQTT_CONNECTED      = BIT_SHIFT(1),
    MQTT_SUBSCRIBED     = BIT_SHIFT(2),
    MQTT_UNSUBSCRIBED   = BIT_SHIFT(3),
    MQTT_ERROR          = BIT_SHIFT(4),
    MQTT_PUBLISHED      = BIT_SHIFT(5),
    MQTT_NEWDATA        = BIT_SHIFT(6),

    LED_ON              = BIT_SHIFT(7),
    LED_OFF             = BIT_SHIFT(8),
    LED_COLOR           = BIT_SHIFT(9),
    ALARM_OFF           = BIT_SHIFT(10),
    ALARM_ON            = BIT_SHIFT(11),

    SEND_DATA           = BIT_SHIFT(12),

    START               = BIT_SHIFT(13),
    CONFIGURE           = BIT_SHIFT(14),


}flags_t;


/******************************** Variables ********************************/

extern uint32_t flags;

/******************************** Prototypes *******************************/


#endif