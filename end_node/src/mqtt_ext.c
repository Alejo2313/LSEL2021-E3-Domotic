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
 * @brief MQTT interface (SOURCE)
 * @version 0.1
 * @date 22/02/2021
 * 
 * @copyright Copyright (c) 2021
 * 
 */


/******************************** Includes *********************************/

#include "mqtt_ext.h"

/******************************** Defines **********************************/



/******************************** Types   **********************************/



/******************************** Variables ********************************/


char wifi_ssid[32];
char wifi_password[64];
char mqtt_broker_url[64];


static esp_mqtt_client_handle_t client;             //<! MQTT Client struct
static char brokerURL[32];                          //<! MQTT broker URL

uint32_t flags;
static uint32_t* prtFlags = &flags;                 //<! MQTT flags pointer (sintax sugar)

static char topicsSubs[TOPIC_MAX][TOPIC_SIZE];      //<! MQTT topics storage 

// In buffer data 
static char inBuffer[MAX_DATA_SIZE];                //<! Input data
static uint8_t inTopicIndex;                        //<! Input topic index
static uint16_t inDataSize;                         //<! Input data size 

static char* TAG = "MQTT";                          //<! LOG header 

/******************************** Prototypes *******************************/



/******************************** Functions ********************************/

/**
 * @brief Get the Income Data from MQTT
 * 
 * @param data Pointer to the data string
 * @param size Data length (Bytes)
 * @return int Topic index
 */
int getIncomeData(char** data, uint16_t* size)
{
    *data = inBuffer;
    *size = inDataSize;
    return inTopicIndex;
}

/**
 * @brief Set flags pinter (FUTURE USE)
 * 
 */
void setFlagsPointer(uint32_t* cflags /* ADD MUTEX */)
{

}

/**
 * @brief Set the Broker URL (FUTURE USE)
 * 
 * @param url Broker URL
 */
void setBrokerURL(const char* url)
{
    if(url != NULL)
    {
        strcpy(brokerURL, url);
    }
}

/**
 * @brief Get the Topic Index 
 * 
 * @param topic Topic to search
 * @return int  Topic index
 */
int getTopicIndex ( const char* topic )
{
    int cnt = 0;
    for(cnt = 0; cnt < NTOPICS; cnt++)
    {
        if( strcmp(topic, topicsSubs[cnt]) == 0 )
        {
            return cnt;
        }
    }
    return -1;
}

/**
 * @brief Subcribe to given topic
 * 
 * @param topic Topic to suscribe
 * @return int  Assigned topic index
 */
void topic_subscribe( const char* topic, uint8_t topicIndex )
{
    if ( IS_FLAG(prtFlags, MQTT_CONNECTED | WIFI_CONNECTED) && topicIndex < TOPIC_MAX )
    {
        esp_mqtt_client_subscribe( client, topic, 0 );
        strcpy( topicsSubs[topicIndex], topic );
    }

};

/**
 * @brief Publish data into specific topic
 * 
 * @param topicId Topic index
 * @param data    Data 
 * @param len     Data length
 * @return esp_err_t status 
 */
esp_err_t topic_publish(int topicId, const char* data, uint16_t len )
{
    if ( IS_FLAG(prtFlags, MQTT_CONNECTED | WIFI_CONNECTED) )
    {
        esp_mqtt_client_publish(client, topicsSubs[topicId], data, len, 0, 0);
        return ESP_OK;
    }
    return ESP_FAIL;
}

/**
 * @brief MQTT Event handler
 * 
 * @param handler_args  MQTT Handler arguments
 * @param base          Pointer to a subsystem that exposes events  
 * @param event_id      Evente identiier
 * @param event_data    Received data
 */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;
    int msg_id;
    char tmp[32];
    if ( prtFlags == NULL)
    {
        return;
    }

    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "CONNECTED");
            SET_FLAGS(prtFlags, MQTT_CONNECTED);
            break;
        case MQTT_EVENT_DISCONNECTED:
            CLEAR_FLAGS(prtFlags, MQTT_CONNECTED);
            break;

        case MQTT_EVENT_SUBSCRIBED:
            SET_FLAGS(prtFlags, MQTT_SUBSCRIBED);
            break;

        case MQTT_EVENT_UNSUBSCRIBED:
            SET_FLAGS(prtFlags, MQTT_UNSUBSCRIBED);
            break;

        case MQTT_EVENT_PUBLISHED:
            SET_FLAGS(prtFlags, MQTT_PUBLISHED);
            break;
            
        case MQTT_EVENT_DATA:
            
            
            sprintf(tmp, "%.*s", event->topic_len, event->topic);
            msg_id = getTopicIndex(tmp);

            if( msg_id == -1 )
            {
                ESP_LOGI(TAG, "TOPIC NO DEFINED!tmp %s", tmp);
                break;
            }

            
            memset(inBuffer,0, MAX_DATA_SIZE);
            strncpy(inBuffer, event->data, event->data_len);
            ESP_LOGI(TAG, "NEW DATA! id %d data %s len %d",msg_id, inBuffer, event->data_len);
            inDataSize = event->data_len;
            inTopicIndex = msg_id;

            SET_FLAGS(prtFlags, MQTT_NEWDATA);
            
            break;
        case MQTT_EVENT_ERROR:
            SET_FLAGS(prtFlags, MQTT_ERROR);
            break;
        default:
            break;
    }
   
}



/**
 * @brief Start MQTT APP
 * 
 */
void mqtt_start(void)
{
 
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = mqtt_broker_url,
    };

    ESP_LOGI(TAG, "CONFIG MQTT!");
    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
    ESP_LOGI(TAG, "CONFIGURED!");

}

void mqtt_stop(void)
{
    esp_mqtt_client_stop(client);
}