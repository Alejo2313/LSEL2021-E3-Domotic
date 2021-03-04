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
 * @file mqtt_ext.h
 * @author Alejandro Gómez (Alejo2313)
 * @author Jaime...
 * @brief MQTT interface (HEADER)
 * @version 0.1
 * @date 22/02/2021
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef __MQTT_EXT__
#define __MQTT_EXT__

/******************************** Includes *********************************/


#include "defines.h"
#include "mqtt_client.h"
#include "platform.h"


/******************************** Defines **********************************/

#define TOPIC_SIZE      32  //<! Max topic length        
#define TOPIC_MAX       7   //<! Max amount of suscribed topics
#define MAX_DATA_SIZE   32  //<! Max data size

/******************************** Types   **********************************/



/******************************** Variables ********************************/



/******************************** Prototypes *******************************/

/**
 * @brief Start MQTT APP
 * 
 */
void mqtt_start(void);

/**
 * @brief Get the Income Data from MQTT
 * 
 * @param data Pointer to the data string
 * @param size Data length (Bytes)
 * @return int Topic index
 */
int getIncomeData( char** data, uint16_t* size );

/**
 * @brief Set flags pinter (FUTURE USE)
 * 
 */
void setFlagsPointer( uint32_t* flags /* ADD MUTEX */);

/**
 * @brief Set the Broker URL (FUTURE USE)
 * 
 * @param url Broker URL
 */
void setBrokerURL( const char* url );

/**
 * @brief Get the Topic Index 
 * 
 * @param topic Topic to search
 * @return int  Topic index
 */
int getTopicIndex ( const char* topic );

/**
 * @brief Subcribe to given topic
 * 
 * @param topic Topic to suscribe
 * @return int  Assigned topic index
 */
int topic_subscribe( const char* topic );

/**
 * @brief Publish data into specific topic
 * 
 * @param topicId Topic index
 * @param data    Data 
 * @param len     Data length
 * @return esp_err_t status 
 */
esp_err_t topic_publish(int topicId, const char* data, uint16_t len );

#endif