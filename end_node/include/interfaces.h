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
 * @file mqtt_ext.c
 * @author Alejandro Gómez (Alejo2313)
 * @author Jaime ....
 * @brief System interfaces (HEADER)
 * @version 0.1
 * @date 28/02/2021
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __INTERFACES_H__
#define __INTERFACES_H__

#include "defines.h"

/****************************  Generic structuture ****************************/

typedef struct 
{
    fsm_t fsm;
    uint32_t * flags;
}fsm_generic_t;


/****************************  LED structutures ****************************/

/**
 * @brief LED function table
 * 
 */
typedef struct 
{
    void (*setGPIO)(uint32_t GPIO);                     //<! Set GPIO to HIGH
    void (*resetGPIO)(uint32_t GPIO);                   //<! Set GPIO to  LOW
    void (*pwmSet)(uint32_t GPIO, uint8_t dutyCycle);   //<! Set PWM duty Cycle 

}led_interface_t;

/**
 * @brief LED Data
 * 
 */
typedef struct 
{
    
    uint32_t* flags;                        //<! pointer to system flags

    uint32_t rPin, gPin, bPin, alarmPin;    //<! GPIO numbers
    uint8_t rColor, gColor, bColor;         //<! LED colours               

}led_data_t;

/**
 * @brief LED fsm Struct 
 * 
 */
typedef struct 
{
    fsm_t fsm;
    led_data_t data;
    led_interface_t interface;

}fsm_led_t;

/****************************  control structutures ****************************/

/**
 * @brief control function table
 * 
 */
typedef struct 
{

    int  (*readGPIO)(uint32_t btnPin);      //<! Read GPIO state
    void (*wifiConnect)();                  //<! Connect to WiFi
    void (*mqttConnect)();                  //<! Connect to MQTT

    void (*enterConfigMode)();              //<! Enter in Staton Mode
    uint64_t (*getTickCount)();             //<! Get tickcount value

}control_interface_t;

/**
 * @brief control Data
 * 
 */
typedef struct 
{

    uint32_t* flags;                //<! Pointer to flags

    uint32_t btnPin;                //<! Button GPIO
    uint64_t tickCounter;           //<! System Tick counter


}control_data_t;

/**
 * @brief Control fsm structure 
 * 
 */
typedef struct 
{
    fsm_t fsm;
    control_data_t data;
    control_interface_t interface;
    
}fsm_control_t;

/****************************  sensor structutures ****************************/

/**
 * @brief Sensor function table
 * 
 */
typedef struct 
{

    int (*readTemp)();              //<! Get current temperature
    int (*readHum)();               //<! Get current humidity
    int (*readLight)();             //<! Get current light

    uint64_t (*getTickCount)();     //<! Get tickcount value

}sensor_interface_t;

/**
 * @brief Sensor data structure
 * 
 */
typedef struct 
{

    uint32_t* flags;                //<! Pointer to flags

    int temp, hum, light;           //<! Sensor values
    uint64_t tickCounter;           //<! System Tick counter

}sensor_data_t;

/**
 * @brief fsm Sensor structure 
 * 
 */
typedef struct 
{
    fsm_t fsm;
    sensor_data_t data;
    sensor_interface_t interface;
    
}fsm_sensor_t;

/****************************  Event structutures ****************************/

/**
 * @brief Event function table 
 * 
 */
typedef struct 
{

    int (*getData)( char** data, uint16_t* size );                          //<! Get input data
    void (*sendData)(int topicId, const char* data, uint16_t len );         //<! Send data
    void (*delayMs)(uint16_t ms);

}event_interface_t;

/**
 * @brief Event data 
 * 
 */
typedef struct 
{
    uint32_t* flags;                //<!  Pointer to flags

    sensor_data_t* sensorData;      //<! Sensor data pointer 
    led_data_t* colorLEDData;       //<! Led data pointer 
    int topic_index[NTOPICS];

}event_data_t;

/**
 * @brief Event FSM structure
 * 
 */
typedef struct 
{
    fsm_t fsm;
    event_data_t data;
    event_interface_t interface;

}fsm_event_t;


#endif