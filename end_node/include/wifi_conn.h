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
 * @file wifi_conn.h
 * @author Alejandro Gómez (Alejo2313)
 * @author Jaime ....
 * @brief WiFi connection functions (HEADER)
 * @version 0.1
 * @date 22/02/2021
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef __WIFI_CONN_H__
#define __WIFI_CONN_H__

/******************************** Includes *********************************/

#include "defines.h"

/******************************** Defines **********************************/

#define MAXIMUM_RETRY       20      //<! Max number of connection retries
#define WIFI_CONNECTED_BIT  BIT0    //<! WiFi connecter flag
#define WIFI_FAIL_BIT       BIT1    //<! WiFi fail flag

/******************************** Types   **********************************/


/******************************** Variables ********************************/


/******************************** Prototypes *******************************/

/**
 * @brief Starts Wifi connection
 * 
 */
void wifi_init_sta(void);

/**
 * @brief Set the Wifi Credentials (FUTURE USE)
 * 
 * @param ussid  Network SSID
 * @param upassword Network pasword
 */
void setWifiCredentials(const char* ussid, const char* upassword);


#endif