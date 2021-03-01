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
 * @brief FSM functions (SOURCE)
 * @version 0.1
 * @date 28/02/2021
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/******************************** Includes *********************************/

#include "fsm_functions.h"

/******************************** Defines **********************************/

/******************************** Types   **********************************/

/******************************** Variables ********************************/

/******************************** Prototypes *******************************/

/******************************** Functions ********************************/


int checkStart( fsm_t* this )
{

  fsm_led_t* fsm = (fsm_led_t*)this;
	return IS_FLAG(fsm->data.flags, START);

}

int checkOnLed( fsm_t* this )
{

  fsm_led_t* fsm = (fsm_led_t*)this;
	return IS_FLAG(fsm->data.flags, LED_ON);

}

int checkOffLed( fsm_t* this )
{

  fsm_led_t* fsm = (fsm_led_t*)this;
	return IS_FLAG(fsm->data.flags, LED_OFF);

}

int checkColorChange( fsm_t* this )
{

  fsm_led_t* fsm = (fsm_led_t*)this;
	return IS_FLAG(fsm->data.flags, LED_COLOR);

}

int checkSystemReset( fsm_t* this )
{
  fsm_led_t* fsm = (fsm_led_t*)this;
	return !IS_FLAG(fsm->data.flags, START);
}

//funciones de activación

void turnOnLed ( fsm_t* this )
{

  fsm_led_t* fsm = (fsm_led_t*)this;

  CLEAR_FLAGS(fsm->data.flags, LED_ON);

  fsm->interface.pwmSet(fsm->data.rPin, DEFAULT_DUTY);
  fsm->interface.pwmSet(fsm->data.gPin, DEFAULT_DUTY);
  fsm->interface.pwmSet(fsm->data.bPin, DEFAULT_DUTY);
  
}

void changeColor ( fsm_t* this ) 
{

  fsm_led_t* fsm = (fsm_led_t*)this;

  CLEAR_FLAGS(fsm->data.flags, LED_COLOR);

  fsm->interface.pwmSet(fsm->data.rPin, fsm->data.rColor);
  fsm->interface.pwmSet(fsm->data.rPin, fsm->data.gColor);
  fsm->interface.pwmSet(fsm->data.rPin, fsm->data.bColor);

}

void turnOffLed ( fsm_t* this ) 
{

  fsm_led_t* fsm = (fsm_led_t*)this;

  CLEAR_FLAGS(fsm->data.flags, LED_OFF);

  fsm->interface.pwmSet(fsm->data.rPin, 0);
  fsm->interface.pwmSet(fsm->data.gPin, 0);
  fsm->interface.pwmSet(fsm->data.bPin, 0);
}


//GENERIC SENSOR FSM




//funciones de guarda

int checkTimerSensor( fsm_t* this ) 
{
	//devuelve el valor del flag que nos informa de que el timer se ha cumplido

  fsm_sensor_t* fsm = (fsm_sensor_t*)this;

  if( fsm->interface.getTickCount() - fsm->data.tickCounter > SENSOR_TICK_RATE )
  {

    fsm->data.tickCounter = fsm->interface.getTickCount();
    return 1;

  }

  return 0;

}

inline int isTrue( fsm_t* this ) 
{
  return 1;
}

//funciones de activación


void startTimerSensor ( fsm_t* this ) 
{

  fsm_sensor_t* fsm = (fsm_sensor_t*)this;

  fsm->data.tickCounter = fsm->interface.getTickCount();

}


void readData ( fsm_t* this ) 
{

  fsm_sensor_t* fsm = (fsm_sensor_t*)this;

  fsm->data.light = fsm->interface.readLight();
  fsm->data.hum   = fsm->interface.readHum();
  fsm->data.temp  = fsm->interface.readTemp();

}

//Queue¿?

void sendData ( fsm_t* this ) 
{
  fsm_sensor_t* fsm = (fsm_sensor_t*)this;

  SET_FLAGS(fsm->data.flags, SEND_DATA);

  fsm->data.tickCounter = fsm->interface.getTickCount();

}


//EVENTOS FSM

int checkFlagOutData( fsm_t* this ) 
{
  fsm_event_t* fsm = (fsm_event_t*)this;
  return IS_FLAG(fsm->data.flags, SEND_DATA);
}

int checkFlagInData( fsm_t* this ) 
{
  fsm_event_t* fsm = (fsm_event_t*)this;
  return IS_FLAG(fsm->data.flags, MQTT_NEWDATA);
}

/**
 * @brief 
 * 
 * @param this 
 */

void processData(fsm_t* this)
{
  char* data;
  uint16_t size;
  int index;
  int* topic;
  int tmp;
  led_data_t* aux;

  fsm_event_t* fsm = (fsm_event_t*)this;
  topic = fsm->data.topic_index;
  aux = fsm->data.colorLEDData;

  CLEAR_FLAGS(fsm->data.flags, MQTT_NEWDATA);

  index = getIncomeData(&data, &size);
  switch (index)
  {
  case topic[TURN_LED]:
    tmp = atoi(data);
    if (tmp > 0)
    {
      SET_FLAGS(fsm->data.flags, LED_ON);
    } else
    {
      SET_FLAGS(fsm->data.flags, LED_OFF);
    }
    break;

  case topic[TURN_ALARM]:
    tmp = atoi(data);
    if (tmp > 0)
    {
      SET_FLAGS(fsm->data.flags, ALARM_ON);
    } else
    {
      SET_FLAGS(fsm->data.flags, ALARM_OFF);
    }
    break;

  case topic[COLOR_LED]:
    tmp = atoi(data);

    aux->rColor = (tmp & 0x00FF0000) >> 16;
    aux->gColor = (tmp & 0x0000FF00) >> 8;
    aux->bColor = (tmp & 0x000000FF);
  break;

  
  default:
    break;
  }

}

void publishData(fsm_t* this)
{
  char data[64];
  uint16_t len;

  fsm_event_t* fsm = (fsm_event_t*)this;
  CLEAR_FLAGS(fsm->data.flags, SEND_DATA);
  int* topic_id = fsm->data.topic_index;

  len = sprintf(data, "%d", fsm->data.sensorData->hum);
  topic_publish(topic_id[SENSOR_HUM], data, len);
  vTaskDelay(10);

  len = sprintf(data, "%d", fsm->data.sensorData->light);
  topic_publish(topic_id[SENSOR_LIGHT], data, len);
  vTaskDelay(10);

  len = sprintf(data, "%d", fsm->data.sensorData->temp);
  topic_publish(topic_id[SENSOR_TEMP], data, len);
  vTaskDelay(10);
}

