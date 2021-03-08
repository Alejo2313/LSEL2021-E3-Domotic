#include "include/fsm_functions.h"
#include "/var/lib/gems/2.7.0/gems/ceedling-0.31.0/vendor/unity/src/unity.h"








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

        .flags = &flags,

        .hum = 0,

        .light = 0,

        .temp = 0,

        .tickCounter = 0

    },

    .interface =

    {

        .getTickCount = getTickCount,

        .readHum = readHum,

        .readLight = readLight,

        .readTemp = readTemp

    }



};



fsm_led_t fsm_led =

{

    .fsm = &fsm,

    .data =

    {

        .flags = &flags,



        .alarmPin = 1,



        .rPin = 2,

        .gPin = 5,

        .bPin = 3,



        .rColor = 10,

        .gColor = 10,

        .bColor = 10,

    },

    .interface =

    {

        .pwmSet = setPWM,

        .resetGPIO = resetGPIO,

        .setGPIO = setGPIO,

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

        .btnPin = 12,

        .flags = &flags,

        .tickCounter = 0

    },

    .interface =

    {

        .enterConfigMode = enterConfigMode,

        .getTickCount = getTickCount,

        .mqttConnect = mqttConnect,

        .readGPIO = readGPIO,

        .wifiConnect = wifiConnect

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



    ( *(&flags) |= (START) );

    do {if ((flags == START)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(152)));}} while(0);



    flags = 0;



    ( *(&flags) |= (START | LED_COLOR) );

    do {if ((flags == (START | LED_COLOR))) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(157)));}} while(0);



    ( *(&flags) &= ~(START) );



    do {if ((flags == LED_COLOR)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(161)));}} while(0);

    UnityAssertEqualNumber((UNITY_INT)((LED_COLOR)), (UNITY_INT)((( (*(&flags))&(LED_COLOR) ))), (

   ((void *)0)

   ), (UNITY_UINT)(162), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((( (*(&flags))&(START) ))), (

   ((void *)0)

   ), (UNITY_UINT)(163), UNITY_DISPLAY_STYLE_INT);



}





void test_fsm_functions_led(void)

{



    flags = 0xFFFFFFFF;

    do {if ((flags&START)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(172)));}} while(0);





    UnityAssertEqualNumber((UNITY_INT)((START)), (UNITY_INT)((checkStart((fsm_t*)(&fsm_led)))), (

   ((void *)0)

   ), (UNITY_UINT)(175), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((LED_ON)), (UNITY_INT)((checkOnLed((fsm_t*)(&fsm_led)))), (

   ((void *)0)

   ), (UNITY_UINT)(176), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((LED_OFF)), (UNITY_INT)((checkOffLed((fsm_t*)(&fsm_led)))), (

   ((void *)0)

   ), (UNITY_UINT)(177), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((LED_COLOR)), (UNITY_INT)((checkColorChange((fsm_t*)(&fsm_led)))), (

   ((void *)0)

   ), (UNITY_UINT)(178), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((checkSystemReset((fsm_t*)(&fsm_led)))), (

   ((void *)0)

   ), (UNITY_UINT)(179), UNITY_DISPLAY_STYLE_INT);

    ( *(&flags) &= ~(START) );



    UnityAssertEqualNumber((UNITY_INT)((1)), (UNITY_INT)((checkSystemReset((fsm_t*)(&fsm_led)))), (

   ((void *)0)

   ), (UNITY_UINT)(182), UNITY_DISPLAY_STYLE_INT);



    turnOnLed ( (fsm_t*)(&fsm_led) );

    do {if ((dCycle == 50)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(185)));}} while(0);



    changeColor ( (fsm_t*)(&fsm_led) ) ;



    do {if ((dCycle == 10)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(189)));}} while(0);



    turnOffLed ( (fsm_t*)(&fsm_led) ) ;

    do {if ((dCycle == 0)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(192)));}} while(0);





}





void test_fsm_sensor(void)

{

    flags = 0;



    tick = 0;

    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((checkTimerSensor((fsm_t*)(&fsm_sensor)))), (

   ((void *)0)

   ), (UNITY_UINT)(203), UNITY_DISPLAY_STYLE_INT);

    tick += 1000;

    UnityAssertEqualNumber((UNITY_INT)((1)), (UNITY_INT)((checkTimerSensor((fsm_t*)(&fsm_sensor)))), (

   ((void *)0)

   ), (UNITY_UINT)(205), UNITY_DISPLAY_STYLE_INT);

    tick += 1000;

    startTimerSensor((fsm_t*)(&fsm_sensor));

    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((checkTimerSensor((fsm_t*)(&fsm_sensor)))), (

   ((void *)0)

   ), (UNITY_UINT)(208), UNITY_DISPLAY_STYLE_INT);



    readData((fsm_t*)(&fsm_sensor));

    do {if ((fsm_sensor.data.hum == 33)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(211)));}} while(0);

    sendData((fsm_t*)(&fsm_sensor));

    UnityAssertEqualNumber((UNITY_INT)((SEND_DATA)), (UNITY_INT)((( (*(&flags))&(SEND_DATA) ))), (

   ((void *)0)

   ), (UNITY_UINT)(213), UNITY_DISPLAY_STYLE_INT);

    do {if ((tick == fsm_sensor.data.tickCounter)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(214)));}} while(0);



}







void test_fsm_event(void)

{



    flags = 0;



    ( *(&flags) |= (MQTT_NEWDATA) );

    UnityAssertEqualNumber((UNITY_INT)((MQTT_NEWDATA)), (UNITY_INT)((checkFlagInData( (fsm_t*)(&fsm_event) ))), (

   ((void *)0)

   ), (UNITY_UINT)(226), UNITY_DISPLAY_STYLE_INT);



    ( *(&flags) |= (SEND_DATA) );

    UnityAssertEqualNumber((UNITY_INT)((SEND_DATA)), (UNITY_INT)((checkFlagOutData( (fsm_t*)(&fsm_event) ))), (

   ((void *)0)

   ), (UNITY_UINT)(229), UNITY_DISPLAY_STYLE_INT);





    topic_ind =TURN_LED;

    mess_size = sprintf(mess, "1");



    processData( (fsm_t*)(&fsm_event) );

    UnityAssertEqualNumber((UNITY_INT)((LED_ON)), (UNITY_INT)((( (*(&flags))&(LED_ON) ))), (

   ((void *)0)

   ), (UNITY_UINT)(236), UNITY_DISPLAY_STYLE_INT);



    mess_size = sprintf(mess, "0");

    processData( (fsm_t*)(&fsm_event) );

    UnityAssertEqualNumber((UNITY_INT)((LED_OFF)), (UNITY_INT)((( (*(&flags))&(LED_OFF) ))), (

   ((void *)0)

   ), (UNITY_UINT)(240), UNITY_DISPLAY_STYLE_INT);



    topic_ind =TURN_ALARM;

    mess_size = sprintf(mess, "1");

    processData( (fsm_t*)(&fsm_event) );

    UnityAssertEqualNumber((UNITY_INT)((ALARM_ON)), (UNITY_INT)((( (*(&flags))&(ALARM_ON) ))), (

   ((void *)0)

   ), (UNITY_UINT)(245), UNITY_DISPLAY_STYLE_INT);



    mess_size = sprintf(mess, "0");

    processData( (fsm_t*)(&fsm_event) );

    UnityAssertEqualNumber((UNITY_INT)((ALARM_OFF)), (UNITY_INT)((( (*(&flags))&(ALARM_OFF) ))), (

   ((void *)0)

   ), (UNITY_UINT)(249), UNITY_DISPLAY_STYLE_INT);





    topic_ind =COLOR_LED;

    mess_size = sprintf(mess, "5592405");

    processData( (fsm_t*)(&fsm_event) );

    UnityAssertEqualNumber((UNITY_INT)((1)), (UNITY_INT)((( (*(&flags))&(LED_COLOR) ))), (

   ((void *)0)

   ), (UNITY_UINT)(255), UNITY_DISPLAY_STYLE_INT);

    do {if ((fsm_led.data.bColor == 0x54)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(256)));}} while(0);



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

    return 33;

}



int readTemp()

{

    return 33;

}



int readLight()

{

    return 33;

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
