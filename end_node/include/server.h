#ifndef __SERVER_H__
#define __SERVER_H__

#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_eth.h"
#include "html_src.h"
#include "defines.h"
#include "platform.h"

#include <esp_http_server.h>


void stop_webserver(void);
void start_webserver(void);

#endif // !__SERVER_H__