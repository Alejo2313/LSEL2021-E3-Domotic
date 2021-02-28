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

#ifndef __FSM_FUNCTIONS_H__
#define __FSM_FUNCTIONS_H__
/******************************** Defines **********************************/

#include "fsm.h"
#include "interfaces.h"
#include "defines.h"

/******************************** Types   **********************************/

#define DEFAULT_DUTY      50
#define SENSOR_TICK_RATE  1000

/******************************** Variables ********************************/

enum led_state {
  IDLE,
  WAITING,
  LIGHT_ON
};


enum sensor_state {
  IDLE,
  WAIT,
  READ
};

enum sensor_state {
  IDLE,
  COMM
};

/******************************** Prototypes *******************************/

// LED functions
int checkStart ( fsm_t* this );
int checkOnLed ( fsm_t* this );
int checkOffLed ( fsm_t* this );
int checkColorChange ( fsm_t* this );
int checkSystemReset ( fsm_t* this );


void turnOnLed ( fsm_t* this );
void changeColor ( fsm_t* this ) ;
void turnOffLed ( fsm_t* this ) ;

//Sensor Functions
int checkTimerSensor( fsm_t* this );

inline int isTrue( fsm_t* this ) ;
void startTimerSensor ( fsm_t* this );
void readData ( fsm_t* this ) ;
void sendData ( fsm_t* this ) ;

/******************************** Tables ********************************/


static fsm_trans_t sensor_fsm[] = {
  { IDLE,   checkStart,           WAIT,     startTimerSensor  },
  { WAIT,   checkTimerSensor,     READ,     readData          },
  { READ,   isTrue,               WAIT,     sendData          },
  { WAIT,   checkSystemReset,     IDLE,     NULL              },
  { READ,   checkSystemReset,     IDLE,     NULL              },
  {-1, NULL, -1, NULL },
};


static fsm_trans_t led_fsm[] = 
{
  { IDLE,       checkStart,         WAITING,    NULL        },
  { WAITING,    checkOnLed,         LIGHT_ON,   turnOnLed   },
  { LIGHT_ON,   checkColorChange,   LIGHT_ON,   changeColor },
  { LIGHT_ON,   checkOffLed,        WAITING,    turnOffLed  },
  { LIGHT_ON,   checkSystemReset,   IDLE,       turnOffLed  },
  { WAITING,    checkSystemReset,   IDLE,       NULL        },
  {-1, NULL, -1, NULL },
};

//tabla de transiciones



#endif