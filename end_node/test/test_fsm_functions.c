#define TEST 

#ifdef TEST

#include "unity.h"
#include "fsm_functions.h"

#define ALARM_PIN   1
#define RPIN        2
#define BPIN        3
#define GPIN        5


#define BTPIN 12
#define COLOR 10 
#define SENSOR_VAL 33


uint64_t tick = 0;
uint8_t dCycle = 0;
uint32_t gpio = 0;
int gpioVal = 0;
uint32_t flags = 0;
char mess[64];
int topic_ind = 0;
uint16_t mess_size = 0;

fsm_t fsm;


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



fsm_sensor_t fsm_sensor = 
{
    .fsm = &fsm,
    .data = 
    {
        .flags          = &flags,
        .hum            = 0,
        .light          = 0,
        .temp           = 0,
        .tickCounter    = 0
    },
    .interface = 
    {
        .getTickCount   = getTickCount,
        .readHum        = readHum,
        .readLight      = readLight,
        .readTemp       = readTemp
    }
    
};

fsm_led_t fsm_led = 
{
    .fsm = &fsm,
    .data = 
    {
        .flags = &flags,

        .alarmPin = ALARM_PIN,
        
        .rPin     = RPIN,
        .gPin     = GPIN,
        .bPin     = BPIN,

        .rColor   = COLOR,
        .gColor   = COLOR,
        .bColor   = COLOR,
    },
    .interface = 
    {
        .pwmSet     = setPWM,
        .resetGPIO  = resetGPIO,
        .setGPIO    = setGPIO,
    }
    
};

fsm_event_t fsm_event = 
{
    .fsm = &fsm,
    .data = 
    {
        .colorLEDData = &(fsm_led.data),
        .flags = &flags,
        .sensorData = &(fsm_sensor.data),
    },
    .interface = 
    {
        .delayMs = delayMs,
        .getData = getData_mqtt,
        .sendData = sendData_mqtt,
    }
};

fsm_control_t fsm_control = 
{
    .fsm = &fsm,
    .data =
    {
        .btnPin         = BTPIN,
        .flags          = &flags,
        .tickCounter    = 0
    },
    .interface =
    {
        .enterConfigMode = enterConfigMode,
        .getTickCount    = getTickCount,
        .mqttConnect     = mqttConnect,
        .readGPIO        = readGPIO,
        .wifiConnect     = wifiConnect
    }
};




void setUp(void)
{

}


void tearDown(void)
{
}




void test_macros(void)
{
    flags = 0;

    SET_FLAGS(&flags, START);
    TEST_ASSERT(flags == START);

    flags = 0;

    SET_FLAGS(&flags, START | LED_COLOR);
    TEST_ASSERT(flags == (START | LED_COLOR));

    CLEAR_FLAGS(&flags, START);

    TEST_ASSERT(flags == LED_COLOR);
    TEST_ASSERT_EQUAL(LED_COLOR, IS_FLAG(&flags, LED_COLOR));
    TEST_ASSERT_EQUAL(0, IS_FLAG(&flags, START));

}


void test_fsm_functions_led(void)
{
    
    flags = 0xFFFFFFFF;
    TEST_ASSERT(flags&START);


    TEST_ASSERT_EQUAL(START, checkStart((fsm_t*)(&fsm_led)));
    TEST_ASSERT_EQUAL(LED_ON, checkOnLed((fsm_t*)(&fsm_led)));
    TEST_ASSERT_EQUAL(LED_OFF, checkOffLed((fsm_t*)(&fsm_led)));
    TEST_ASSERT_EQUAL(LED_COLOR, checkColorChange((fsm_t*)(&fsm_led)));
    TEST_ASSERT_EQUAL(0, checkSystemReset((fsm_t*)(&fsm_led)));
    CLEAR_FLAGS(&flags, START);

    TEST_ASSERT_EQUAL(1, checkSystemReset((fsm_t*)(&fsm_led)));

    turnOnLed ( (fsm_t*)(&fsm_led) );
    TEST_ASSERT( dCycle == DEFAULT_DUTY );

    changeColor ( (fsm_t*)(&fsm_led) ) ;

    TEST_ASSERT( dCycle == COLOR );

    turnOffLed ( (fsm_t*)(&fsm_led) ) ;
    TEST_ASSERT( dCycle == 0 );


}


void test_fsm_sensor(void)
{
    flags = 0;

    tick = 0;
    TEST_ASSERT_EQUAL(0, checkTimerSensor((fsm_t*)(&fsm_sensor)));
    tick += SENSOR_TICK_RATE;
    TEST_ASSERT_EQUAL(1, checkTimerSensor((fsm_t*)(&fsm_sensor)));
    tick += SENSOR_TICK_RATE;
    startTimerSensor((fsm_t*)(&fsm_sensor));
    TEST_ASSERT_EQUAL(0, checkTimerSensor((fsm_t*)(&fsm_sensor)));

    readData((fsm_t*)(&fsm_sensor));
    TEST_ASSERT(fsm_sensor.data.hum == SENSOR_VAL);
    sendData((fsm_t*)(&fsm_sensor));
    TEST_ASSERT_EQUAL(SEND_DATA, IS_FLAG(&flags, SEND_DATA));
    TEST_ASSERT(tick == fsm_sensor.data.tickCounter);

}



void test_fsm_event(void)
{

    flags = 0;

    SET_FLAGS(&flags, MQTT_NEWDATA);
    TEST_ASSERT_EQUAL(MQTT_NEWDATA, checkFlagInData( (fsm_t*)(&fsm_event) ));

    SET_FLAGS(&flags, SEND_DATA);
    TEST_ASSERT_EQUAL(SEND_DATA, checkFlagOutData( (fsm_t*)(&fsm_event) ));

//
    topic_ind =TURN_LED;
    mess_size = sprintf(mess, "1");

    processData( (fsm_t*)(&fsm_event) );
    TEST_ASSERT_EQUAL(LED_ON, IS_FLAG(&flags, LED_ON));

    mess_size = sprintf(mess, "0");
    processData( (fsm_t*)(&fsm_event) );
    TEST_ASSERT_EQUAL(LED_OFF, IS_FLAG(&flags, LED_OFF));
//
    topic_ind =TURN_ALARM;
    mess_size = sprintf(mess, "1");
    processData( (fsm_t*)(&fsm_event) );
    TEST_ASSERT_EQUAL(ALARM_ON, IS_FLAG(&flags, ALARM_ON));

    mess_size = sprintf(mess, "0");
    processData( (fsm_t*)(&fsm_event) );
    TEST_ASSERT_EQUAL(ALARM_OFF, IS_FLAG(&flags, ALARM_OFF));
//

    topic_ind =COLOR_LED;
    mess_size = sprintf(mess, "5592405");
    processData( (fsm_t*)(&fsm_event) );
    TEST_ASSERT_EQUAL(LED_COLOR, IS_FLAG(&flags, LED_COLOR));
    TEST_ASSERT(fsm_led.data.bColor == 0x55);

}












void setPWM (uint32_t GPIO, uint8_t dutyCycle)
{
    dCycle = dutyCycle;
    gpio = gpio;
    printf ("setting PWM %d on GPIO %d \n", dutyCycle, GPIO);
}


void resetGPIO(uint32_t GPIO)
{
    gpioVal = 0;
    printf ("resetting GPIO %d \n", GPIO);
}

void setGPIO(uint32_t GPIO)
{
    gpioVal = 1;
    printf ("setting GPIO %d \n", GPIO);
}


uint64_t getTickCount()
{
    return tick;
};

int readHum()
{
    return SENSOR_VAL;
}

int readTemp()
{
    return SENSOR_VAL;
}

int readLight()
{
    return SENSOR_VAL;
}

int getData_mqtt( char** data, uint16_t* size )
{
    *data = &mess;
    *size = mess_size;
    return topic_ind;
}                         
void sendData_mqtt(int topicId, const char* data, uint16_t len )
{
    printf("sending %s from %d with %d bytes", data, topicId, len);
}     

void delayMs(uint16_t ms)
{
    return;
}

void enterConfigMode(void)
{
    printf("Entering in config mode \n");
}

void mqttConnect(void)
{
    printf("Connecting to MQTT \n");
}

void wifiConnect(void)
{
    printf("Connecting to WiFi \n");
}

int readGPIO(uint32_t GPIO)
{
    return gpioVal;
}

#endif // TEST
