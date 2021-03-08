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

/**
 * @brief checks if flag START is enabled
 * 
 * @param this: fsm structure used
 * @return int: flag START's value 
 */
int checkStart( fsm_t* this )
{

  fsm_generic_t* fsm = (fsm_generic_t*)this;

	return IS_FLAG(fsm->flags, START);

}

/**
 * @brief checks if flag LED_ON is enabled
 * 
 * @param this: fsm structure used
 * @return int: flag LED_ON's value 
 */
int checkOnLed( fsm_t* this )
{

  fsm_led_t* fsm = (fsm_led_t*)this;
	return IS_FLAG(fsm->data.flags, LED_ON);

}

/**
 * @brief checks if flag LED_OFF is enabled
 * 
 * @param this: fsm structure used
 * @return int: flag LED_OFF's value 
 */
int checkOffLed( fsm_t* this )
{

  fsm_led_t* fsm = (fsm_led_t*)this;
	return IS_FLAG(fsm->data.flags, LED_OFF);

}

/**
 * @brief checks if flag LED_COLOR is enabled
 * 
 * @param this: fsm structure used
 * @return int: flag LED_COLOR's value 
 */
int checkColorChange( fsm_t* this )
{

  fsm_led_t* fsm = (fsm_led_t*)this;
	return IS_FLAG(fsm->data.flags, LED_COLOR);

}

/**
 * @brief checks if flag START is disabled
 * 
 * @param this: fsm structure used
 * @return int: flag START's value inverted 
 */
int checkSystemReset( fsm_t* this )
{
  fsm_led_t* fsm = (fsm_led_t*)this;

	return !IS_FLAG(fsm->data.flags, START);
}


/**
 * @brief turns the leds with the default PWM duty cycle value
 * 
 * @param this fsm structure used
 */
void turnOnLed ( fsm_t* this )
{

  fsm_led_t* fsm = (fsm_led_t*)this;

  CLEAR_FLAGS(fsm->data.flags, LED_ON);

  fsm->interface.pwmSet(fsm->data.rPin, DEFAULT_DUTY);
  fsm->interface.pwmSet(fsm->data.gPin, DEFAULT_DUTY);
  fsm->interface.pwmSet(fsm->data.bPin, DEFAULT_DUTY);
  
}

/**
 * @brief changes the leds colors by modifying the PWM duty cycle
 * 
 * @param this 
 */
void changeColor ( fsm_t* this ) 
{

  fsm_led_t* fsm = (fsm_led_t*)this;

  CLEAR_FLAGS(fsm->data.flags, LED_COLOR);

  fsm->interface.pwmSet(fsm->data.rPin, fsm->data.rColor);
  fsm->interface.pwmSet(fsm->data.gPin, fsm->data.gColor);
  fsm->interface.pwmSet(fsm->data.bPin, fsm->data.bColor);

}

/**
 * @brief turns the leds off by setting the PWM duty cycles to 0
 * 
 * @param this 
 */
void turnOffLed ( fsm_t* this ) 
{

  fsm_led_t* fsm = (fsm_led_t*)this;

  CLEAR_FLAGS(fsm->data.flags, LED_OFF);

  fsm->interface.pwmSet(fsm->data.rPin, 0);
  fsm->interface.pwmSet(fsm->data.gPin, 0);
  fsm->interface.pwmSet(fsm->data.bPin, 0);
}



/**
 * @brief checks whether the time between read operations to sensors is over
 * 
 * @param this fsm structure used
 * @return int 1 if the time is over, 0 if it is not
 */
int checkTimerSensor( fsm_t* this ) 
{
	//devuelve el valor del flag que nos informa de que el timer se ha cumplido

  fsm_sensor_t* fsm = (fsm_sensor_t*)this;

  if( fsm->interface.getTickCount() - fsm->data.tickCounter >= SENSOR_TICK_RATE )
  {

    return 1;

  }

  return 0;

}

/**
 * @brief always returns 1
 * 
 * @param this fsm structure used
 * @return int always true (1)
 */
int isTrue( fsm_t* this ) 
{
  return 1;
}


/**
 * @brief start the timer between read operations of sensors
 * 
 * @param this fsm structure used
 */
void startTimerSensor ( fsm_t* this ) 
{

  fsm_sensor_t* fsm = (fsm_sensor_t*)this;

  fsm->data.tickCounter = fsm->interface.getTickCount();

}

/**
 * @brief reads sensors data and saves them in the appropriate structure
 * 
 * @param this 
 */
void readData ( fsm_t* this ) 
{

  fsm_sensor_t* fsm = (fsm_sensor_t*)this;

  fsm->data.light = fsm->interface.readLight();
  fsm->data.hum   = fsm->interface.readHum();
  fsm->data.temp  = fsm->interface.readTemp();

}

//Queue¿?

/**
 * @brief enables SEND_DATA flag and starts timer between read operations
 * 
 * @param this fsm structure used
 */
void sendData ( fsm_t* this ) 
{
  fsm_sensor_t* fsm = (fsm_sensor_t*)this;

  SET_FLAGS(fsm->data.flags, SEND_DATA);

  fsm->data.tickCounter = fsm->interface.getTickCount();

}


/**
 * @brief checks if SEND_DATA flag is enabled
 * 
 * @param this fsm structure used
 * @return int SEND_DATA's value
 */
int checkFlagOutData( fsm_t* this ) 
{
  fsm_event_t* fsm = (fsm_event_t*)this;
  return IS_FLAG(fsm->data.flags, SEND_DATA);
}

/**
 * @brief checks if MQTT_NEWDATA flag is enabled
 * 
 * @param this fsm structure used
 * @return int MQTT_NEWDATA's value
 */
int checkFlagInData( fsm_t* this ) 
{
  fsm_event_t* fsm = (fsm_event_t*)this;
  return IS_FLAG(fsm->data.flags, MQTT_NEWDATA);
}


/**
 * @brief collects the received data and enables the appropriate flags according to the incoming information  
 * 
 * @param this fsm structure used
 */
void processData(fsm_t* this)
{
  char* data;
  uint16_t size;
  int index;
  int* topic;
  uint32_t tmp;

  led_data_t* aux;

  fsm_event_t* fsm = (fsm_event_t*)this;
  aux = fsm->data.colorLEDData;

  CLEAR_FLAGS(fsm->data.flags, MQTT_NEWDATA);

  index = fsm->interface.getData(&data, &size);

  switch (index)
  {
  case TURN_LED:
    tmp = atoi(data);
    if (tmp > 0)
    {
      SET_FLAGS(fsm->data.flags, LED_ON);
    } 
    else
    {
      SET_FLAGS(fsm->data.flags, LED_OFF);
    }
    break;

  case TURN_ALARM:
    tmp = atoi(data);
    if (tmp > 0)
    {
      SET_FLAGS(fsm->data.flags, ALARM_ON);
    } 
    else
    {
      SET_FLAGS(fsm->data.flags, ALARM_OFF);
    }
    break;

  case COLOR_LED:
    tmp = atoi(data);

    SET_FLAGS(fsm->data.flags, LED_COLOR);
    
    aux->rColor = (tmp & 0x00FF0000) >> 16;
    aux->gColor = (tmp & 0x0000FF00) >> 8;
    aux->bColor = (tmp & 0x000000FF);
  break;

  
  default:
    break;
  }

}

/**
 * @brief sends the sensor's values collected previously to the gateway
 * 
 * @param this fsm structure used
 */
void publishData(fsm_t* this)
{
  char data[64];
  uint16_t len;

  fsm_event_t* fsm = (fsm_event_t*)this;
  CLEAR_FLAGS(fsm->data.flags, SEND_DATA);

  len = sprintf(data, "%d", fsm->data.sensorData->hum);
  fsm->interface.sendData(SENSOR_HUM, data, len);
  fsm->interface.delayMs(10);

  len = sprintf(data, "%d", fsm->data.sensorData->light);
  fsm->interface.sendData(SENSOR_LIGHT, data, len);
  fsm->interface.delayMs(10);

  len = sprintf(data, "%d", fsm->data.sensorData->temp);
  fsm->interface.sendData(SENSOR_TEMP, data, len);
  fsm->interface.delayMs(10);
}



/**
 * @brief checks if the device is both connected to MQTT and WiFi
 * 
 * @param this fsm structure used
 * @return int 1 if it is connected, 0 if any of the connections is down
 */
int checkConnected(fsm_t* this)
{
  fsm_control_t* fsm = (fsm_control_t*)this;

  return IS_FLAG(fsm->data.flags, MQTT_CONNECTED) && IS_FLAG(fsm->data.flags, WIFI_CONNECTED);
}


/**
 * @brief checks if the device is both connected to MQTT and WiFi
 * 
 * @param this fsm structure used
 * @return int 0 if it is connected, 1 if any of the connections is down
 */
int checkNotConnected(fsm_t* this)
{
  fsm_control_t* fsm = (fsm_control_t*)this;
  return !(IS_FLAG(fsm->data.flags, MQTT_CONNECTED) && IS_FLAG(fsm->data.flags, WIFI_CONNECTED));
}

/**
 * @brief checks if the device is appropriately configured
 * 
 * @param this fsm structure used
 * @return int 1 if it is configured, 0 if not
 */
int checkNotConfigured(fsm_t* this)
{
  fsm_control_t* fsm = (fsm_control_t*)this;
  return !IS_FLAG(fsm->data.flags, CONFIGURE);
}

/**
 * @brief checks if the button is being pressed
 * 
 * @param this fsm structure used
 * @return int 1 if it is being pressed, 0 if not
 */
int checkButton(fsm_t* this)
{
  fsm_control_t* fsm = (fsm_control_t*)this;
  uint32_t btnPin = fsm->data.btnPin;
  
  return fsm->interface.readGPIO(btnPin);
}

/**
 * @brief checks if the timer associated to the button has reached a predefined value
 * 
 * @param this fsm structure used
 * @return int 1 if it has, 0 if not
 */
int checkTimerHigher(fsm_t* this)
{
  fsm_control_t* fsm = (fsm_control_t*)this;

  if( fsm->interface.getTickCount() - fsm->data.tickCounter >= BUTTON_TIME )
  {
    return 1;
  }
  return 0;
}

/**
 * @brief checks if the timer associated to the button has reached a predefined value
 * 
 * @param this fsm structure used
 * @return int 0 if it has, 1 if not
 */
int checkTimerLower(fsm_t* this)
{
  fsm_control_t* fsm = (fsm_control_t*)this;

  if( fsm->interface.getTickCount() - fsm->data.tickCounter < BUTTON_TIME )
  {
    return 1;
  }
  return 0;
}

/**
 * @brief checks if the timer associated to the button has reached a predefined value, and the button is being pressed at the same time
 * 
 * @param this fsm structure used
 * @return int 1 if it has not and the button is being pressed, 0 otherwise
 */
int checkNotButtonTimerLower(fsm_t* this)
{
  fsm_control_t* fsm = (fsm_control_t*)this;
  uint32_t btnPin = fsm->data.btnPin;

  if( (fsm->interface.getTickCount() - fsm->data.tickCounter < BUTTON_TIME) && (!fsm->interface.readGPIO(btnPin)))
  {
    return 1;
  }
    return 0;
}

/**
 * @brief sets both MQTT and WiFi flags to 1
 * 
 * @param this fsm structure used
 */
void enableConnect(fsm_t* this)
{
  fsm_control_t* fsm = (fsm_control_t*)this;
  fsm->interface.mqttConnect();
  fsm->interface.wifiConnect();
  CLEAR_FLAGS(fsm->data.flags, START);
}

/**
 * @brief sets CONFIGURE flag to 1
 * 
 * @param this fsm structure used
 */
void enableEnterConfig(fsm_t* this)
{
  fsm_control_t* fsm = (fsm_control_t*)this;
  SET_FLAGS(fsm->data.flags, CONFIGURE);
}

/**
 * @brief starts the timer associated to the button
 * 
 * @param this fsm structure used
 */
void startTimerButton (fsm_t* this) 
{
  fsm_control_t* fsm = (fsm_control_t*)this;
  fsm->data.tickCounter = fsm->interface.getTickCount();
}

/**
 * @brief sets START flag to 1
 * 
 * @param this fsm structure used
 */
void enableStart(fsm_t* this)
{
  fsm_control_t* fsm = (fsm_control_t*)this;
  SET_FLAGS(fsm->data.flags, START);
}