#include "fsm.h"
#include "interfaces.h"
#include "defines.h"

//structs and variables

LED_interface_t led_interface;
flags_t flags;
LED_data_t led_data;
SENSOR_data_t sensor_data;
SENSOR_interface_t sensor_interface;
Event_data_t event_data;
Event_interface_t event_interface;


//LED FSM FUNCTIONS

enum led_state {
  IDLE,
  WAITING,
  LED_ON
};

//funciones de guarda

int compruebaStart(fsm_t* this) {
	return flags.START;
}

int compruebaOnLed(fsm_t* this) {
	return flags.LED_ON;
}

int compruebaOffLed(fsm_t* this) {
	return flags.LED_OFF;
}

int compruebaVariacionColor(fsm_t* this) {
	return flags.LED_COLOR;
}

int compruebaResetSystem(fsm_t* this) {
	return flags.CONFIGURE;
}

//funciones de activación

void encenderLed (fsm_t* this) {
	led_interface.gpioSet();
}

void regularIntensidadLed (fsm_t* this) {
	led_interface.pwmSet(led_data.rPin, led_data.rColor);
   	led_interface.pwmSet(led_data.bPin, led_data.bColor);
	led_interface.pwmSet(led_data.gPin, led_data.gColor);
}

void apagarLed (fsm_t* this) {
	led_interface.gpioReset();
}


//tabla de transiciones

static fsm_trans_t led_fsm[] = {
  { IDLE,       compruebaStart,                 WAITING,    NULL                },
  { WAITING,    compruebaOnLed,                 LED_ON,     encenderLed         },
  { LED_ON,     compruebaVariacionIntensidad,   LED_ON,     regularIntensidadLed},
  { LED_ON,     compruebaOffLed,                WAITING,    apagarLed           },
  { LED_ON,     compruebaResetSystem,           IDLE,       NULL                },
  { WAITING,    compruebaResetSystem,           IDLE,       NULL                },
  {-1, NULL, -1, NULL },
};


//GENERIC SENSOR FSM

enum sensor_state {
  IDLE,
  WAIT,
  READ
};


//funciones de guarda

int compruebaTimerSensor(fsm_t* this) {
	//devuelve el valor del flag que nos informa de que el timer se ha cumplido
}

int compruebaDataAvailable(fsm_t* this) {
	//devuelve el valor del flag que nos informa de que los datos están listos para enviarse
}


//funciones de activación


void startTimerSensor (fsm_t* this) {
	//Comienza la cuenta del timer del sensor
}


void readData (fsm_t* this) {
	sensor_data.hum = sensor_interface.readHum();
    sensor_data.light = sensor_interface.readLight();
    sensor_data.temp = sensor_interface.readTemp();
}


void sendData (fsm_t* this) {
    //levantamos flag indicando que se pueden mandar nuevos datos, empezamos el timer de nuevo
}

//tabla de transiciones

static fsm_trans_t sensor_fsm[] = {
  { IDLE,   compruebaStart,           WAIT,     startTimerSensor    },
  { WAIT,   compruebaTimerSensor,     READ,     readData            },
  { READ,   compruebaDataAvailable,   WAIT,     sendData            },
  { WAIT,   compruebaResetSystem,     IDLE,     NULL                },
  { READ,   compruebaResetSystem,     IDLE,     NULL                },
  {-1, NULL, -1, NULL },
};


//EVENTOS FSM

enum sensor_state {
  IDLE,
  COMM
};


//funciones de guarda

int compruebaFlagSensor(fsm_t* this) {
	//comprueba si hay nueva información de los sensores para enviar
}

int compruebaFlagEnciendeLed(fsm_t* this, flags_t flags) {
    return flags.LED_ON;
}

int compruebaFlagApagaLed(fsm_t* this, flags_t flags) {
    return flags.LED_OFF;
}

int compruebaFlagColorLed(fsm_t* this, flags_t flags) {
    return flags.LED_COLOR;
}

int compruebaFlagAlarmaOn(fsm_t* this, flags_t flags) {
    return flags.ALARM_ON;
}

int compruebaFlagAlarmaOff(fsm_t* this, flags_t flags) {
    return flags.ALARM_OFF;
}

//funciones de activación


void sendDataSensores (fsm_t* this) {
    //Enviamos los datos recogidos por los sensores
}


void onLed (fsm_t* this) {
	//Damos la orden de encender los leds
}


void offLed (fsm_t* this) {
    //Damos la orden de apagar los leds
}

void colorLed (fsm_t* this) {
    //Damos la orden de cambiar el color de los leds
}


void alarmaOn (fsm_t* this) {
    //Damos la orden de encender la alarma
}

void alarmaOff (fsm_t* this) {
    //Damos la orden de apagar la alarma
}


//tabla de transiciones

static fsm_trans_t eventos_fsm[] = {
  { IDLE,   compruebaStart,             COMM,     NULL              },
  { COMM,   compruebaFlagSensor,        COMM,     sendDataSensores  },
  { COMM,   compruebaFlagEnciendeLed,   COMM,     onLed             },
  { COMM,   compruebaFlagApagaLed,      COMM,     offLed            },
  { COMM,   compruebaFlagColorLed,      COMM,     colorLed          },
  { COMM,   compruebaFlagAlarmaOn,      COMM,     alarmaOn          },
  { COMM,   compruebaFlagAlarmaOff,     COMM,     alarmaOff         },
  { COMM,   compruebaResetSystem,       IDLE,     NULL              },
  {-1, NULL, -1, NULL },
};


