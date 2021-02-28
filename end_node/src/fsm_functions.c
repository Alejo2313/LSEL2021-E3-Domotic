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

//tabla de transiciones




//EVENTOS FSM



/*** REVISAR
//funciones de guarda

int checkFlagSensor( fsm_t* this ) {
	//check si hay nueva información de los sensores para enviar
}

int checkFlagEnciendeLed(fsm_t* this, flags_t flags) {
    return flags.LED_ON;
}

int checkFlagApagaLed(fsm_t* this, flags_t flags) {
    return flags.LED_OFF;
}

int checkFlagColorLed(fsm_t* this, flags_t flags) {
    return flags.LED_COLOR;
}

int checkFlagAlarmaOn(fsm_t* this, flags_t flags) {
    return flags.ALARM_ON;
}

int checkFlagAlarmaOff(fsm_t* this, flags_t flags) {
    return flags.ALARM_OFF;
}

//funciones de activación


void sendDataSensores ( fsm_t* this ) {
    //Enviamos los datos recogidos por los sensores
}


void onLed ( fsm_t* this ) {
	//Damos la orden de encender los leds
}


void offLed ( fsm_t* this ) {
    //Damos la orden de apagar los leds
}

void colorLed ( fsm_t* this ) {
    //Damos la orden de cambiar el color de los leds
}


void alarmaOn ( fsm_t* this ) {
    //Damos la orden de encender la alarma
}

void alarmaOff ( fsm_t* this ) {
    //Damos la orden de apagar la alarma
}


//tabla de transiciones

static fsm_trans_t eventos_fsm[] = {
  { IDLE,   checkStart,             COMM,     NULL              },
  { COMM,   checkFlagSensor,        COMM,     sendDataSensores  },
  { COMM,   checkFlagEnciendeLed,   COMM,     onLed             },
  { COMM,   checkFlagApagaLed,      COMM,     offLed            },
  { COMM,   checkFlagColorLed,      COMM,     colorLed          },
  { COMM,   checkFlagAlarmaOn,      COMM,     alarmaOn          },
  { COMM,   checkFlagAlarmaOff,     COMM,     alarmaOff         },
  { COMM,   checkSystemReset,       IDLE,     NULL              },
  {-1, NULL, -1, NULL },
};

*/
