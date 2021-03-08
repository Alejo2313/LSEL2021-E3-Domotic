#ifndef __PLATFORM_H__
#define __PLATFORM_H__

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



void setPWM (uint32_t GPIO, uint8_t dutyCycle);
void resetGPIO(uint32_t GPIO);
void setGPIO(uint32_t GPIO);
uint64_t getTickCount();
int readHum();
int readTemp();
int readLight();
int getData_mqtt( char** data, uint16_t* size );                          
void sendData_mqtt(int topicId, const char* data, uint16_t len );         
void delayMs(uint16_t ms);
void enterConfigMode(void);
void mqttConnect(void);
void wifiConnect(void);
int readGPIO(uint32_t GPIO);




#endif // DEBUG