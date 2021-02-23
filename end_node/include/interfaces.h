#ifndef __INTERFACES_H__
#define __INTERFACES_H__

#include "defines.h"

typedef struct 
{
    void (*gpioSet)(void);
    void (*gpioReset)(void);

    void (*pwmSet)(uint32_t GPIO, uint8_t dutyCycle);

}LED_interface_t;

typedef struct 
{
    
    uint32_t* flags;
//    void* mutex; ??
    uint32_t rPin, gPin, bPin;
    uint8_t rColor, gColor, bColor;
    uint32_t alarmPin;

}LED_data_t;


typedef struct 
{

    int  (*readGPIO)(uint32_t btnPin);
    void (*wifiConnect)();
    void (*mqttConnect)();

    void (*enterConfigMode)();
    uint64_t (*getTickCount)();

}CONTROL_interface_t;

typedef struct 
{

    uint32_t* flags;
    uint32_t btnPin;

}CONTROL_data_t;


typedef struct 
{

    int (*readTemp)();
    int (*readHum)();
    int (*readLight)();

    uint64_t (*getTickCount)();

}SENSOR_interface_t;

typedef struct 
{

    uint32_t* flags;
    int temp, hum, light;

}SENSOR_data_t;



typedef struct 
{

    int (*getData)( char** data, uint16_t* size );
    void (*sendData)(int topicId, const char* data, uint16_t len );

}Event_interface_t;

typedef struct 
{
    uint32_t* flags;

    uint32_t* uint8_t colorLED;
    SENSOR_data_t* sensorData;
    LED_data_t* colorLEDData;

}Event_data_t;



#endif