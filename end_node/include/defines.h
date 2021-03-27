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
#include <stdlib.h>
#include <string.h>

#include "fsm.h"

/******************************** Defines **********************************/

#define BIT_SHIFT(n) (1 << (n))
#define CLEAR_FLAGS(ptrFlags, flag) ( *(ptrFlags) &= ~(flag) )
#define SET_FLAGS(ptrFlags, flag)   ( *(ptrFlags) |= (flag) )
#define IS_FLAG(ptrFlags, flag) ( (*(ptrFlags))&(flag) )

/*
#define CONFIG_BROKER_URL    "mqtt://51.103.29.76:1883"
#define SSID     "test"
#define PASSWORD "test12345"
*/
#define NTOPICS    6


extern char wifi_ssid[32];
extern char wifi_password[64];
extern char mqtt_broker_url[64];

#define DEFAULT_SSID        "test"
#define DEFAULT_PASSWORD    "test12345"
#define DEFAULT_BROKER      "mqtt://51.103.29.76:1883"


#define DEVICE_ID       "ABCD"
#define BASE_TOPIC      "/HOME/"

#define TYPE_INT            "-I"
#define TYPE_STRING         "-D"
#define TYPE_DOUBLE         "-S"

#define TYPE_TURN_LED        "-0" 
#define TYPE_TURN_ALARM      "-1" 
#define TYPE_COLOR_LED       "-2" 
#define TYPE_SENSOR_TEMP     "-3" 
#define TYPE_SENSOR_HUM      "-4"
#define TYPE_SENSOR_LIGHT    "-5"  

#define TYPE_SENSOR          "-I"
#define TYPE_ACTUATOR        "-O"

#define TURN_LED_TOPIC          BASE_TOPIC  DEVICE_ID    TYPE_INT   TYPE_TURN_LED     TYPE_ACTUATOR
#define TURN_ALARM_TOPIC        BASE_TOPIC  DEVICE_ID    TYPE_INT   TYPE_TURN_ALARM   TYPE_ACTUATOR 
#define COLOR_LED_TOPIC         BASE_TOPIC  DEVICE_ID    TYPE_INT   TYPE_COLOR_LED    TYPE_ACTUATOR 
#define SENSOR_TEMP_TOPIC       BASE_TOPIC  DEVICE_ID    TYPE_INT   TYPE_SENSOR_TEMP  TYPE_SENSOR
#define SENSOR_HUM_TOPIC        BASE_TOPIC  DEVICE_ID    TYPE_INT   TYPE_SENSOR_HUM   TYPE_SENSOR
#define SENSOR_LIGHT_TOPIC      BASE_TOPIC  DEVICE_ID    TYPE_INT   TYPE_SENSOR_LIGHT TYPE_SENSOR

#define FLAGS_ALL               0xFFFFFFFF
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





typedef enum
{
    TURN_LED        = 0,
    TURN_ALARM      = 1,
    COLOR_LED       = 2,
    SENSOR_TEMP     = 3,
    SENSOR_HUM      = 4,
    SENSOR_LIGHT    = 5,
    SENSOR_PRESS    = 6
}topicex_t;

/******************************** Variables ********************************/

extern uint32_t flags;

/******************************** Prototypes *******************************/


#endif