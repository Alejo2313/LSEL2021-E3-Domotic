#include "include/fsm.h"
#include "include/fsm_functions.h"
#include "/var/lib/gems/2.7.0/gems/ceedling-0.31.0/vendor/unity/src/unity.h"








uint64_t tick = 0;

uint8_t dCycle = 0;

uint32_t gpio = 0;

int gpioVal = 0;

static uint32_t flags_test = 0;

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

    .data =

    {

        .flags = &flags_test,

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



    .data =

    {

        .flags = &flags_test,



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



    .data =

    {

        .colorLEDData = &(fsm_led.data),

        .flags =&flags_test,

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



    .data =

    {

        .btnPin = 12,

        .flags = &flags_test,

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

    flags_test =0;



    ( *(&flags_test) |= (START) );

    do {if ((flags_test == START)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(153)));}} while(0);



    flags_test =0;



    ( *(&flags_test) |= (START | LED_COLOR) );

    do {if ((flags_test == (START | LED_COLOR))) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(158)));}} while(0);



    ( *(&flags_test) &= ~(START) );



    do {if ((flags_test == LED_COLOR)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(162)));}} while(0);

    UnityAssertEqualNumber((UNITY_INT)((LED_COLOR)), (UNITY_INT)((( (*(&flags_test))&(LED_COLOR) ))), (

   ((void *)0)

   ), (UNITY_UINT)(163), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((( (*(&flags_test))&(START) ))), (

   ((void *)0)

   ), (UNITY_UINT)(164), UNITY_DISPLAY_STYLE_INT);



}





void test_fsm_functions_led(void)

{



    flags_test =0xFFFFFFFF;

    do {if ((flags_test & START)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(173)));}} while(0);





    UnityAssertEqualNumber((UNITY_INT)((START)), (UNITY_INT)((checkStart((fsm_t*)(&fsm_led)))), (

   ((void *)0)

   ), (UNITY_UINT)(176), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((LED_ON)), (UNITY_INT)((checkOnLed((fsm_t*)(&fsm_led)))), (

   ((void *)0)

   ), (UNITY_UINT)(177), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((LED_OFF)), (UNITY_INT)((checkOffLed((fsm_t*)(&fsm_led)))), (

   ((void *)0)

   ), (UNITY_UINT)(178), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((LED_COLOR)), (UNITY_INT)((checkColorChange((fsm_t*)(&fsm_led)))), (

   ((void *)0)

   ), (UNITY_UINT)(179), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((checkSystemReset((fsm_t*)(&fsm_led)))), (

   ((void *)0)

   ), (UNITY_UINT)(180), UNITY_DISPLAY_STYLE_INT);

    ( *(&flags_test) &= ~(START) );



    UnityAssertEqualNumber((UNITY_INT)((1)), (UNITY_INT)((checkSystemReset((fsm_t*)(&fsm_led)))), (

   ((void *)0)

   ), (UNITY_UINT)(183), UNITY_DISPLAY_STYLE_INT);



    turnOnLed ( (fsm_t*)(&fsm_led) );

    do {if ((dCycle == 50)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(186)));}} while(0);



    changeColor ( (fsm_t*)(&fsm_led) ) ;



    do {if ((dCycle == 10)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(190)));}} while(0);



    turnOffLed ( (fsm_t*)(&fsm_led) ) ;

    do {if ((dCycle == 0)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(193)));}} while(0);





}





void test_fsm_sensor(void)

{

    flags_test =0;



    tick = 0;

    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((checkTimerSensor((fsm_t*)(&fsm_sensor)))), (

   ((void *)0)

   ), (UNITY_UINT)(204), UNITY_DISPLAY_STYLE_INT);

    tick += 1000;

    UnityAssertEqualNumber((UNITY_INT)((1)), (UNITY_INT)((checkTimerSensor((fsm_t*)(&fsm_sensor)))), (

   ((void *)0)

   ), (UNITY_UINT)(206), UNITY_DISPLAY_STYLE_INT);

    tick += 1000;

    startTimerSensor((fsm_t*)(&fsm_sensor));

    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((checkTimerSensor((fsm_t*)(&fsm_sensor)))), (

   ((void *)0)

   ), (UNITY_UINT)(209), UNITY_DISPLAY_STYLE_INT);



    readData((fsm_t*)(&fsm_sensor));

    do {if ((fsm_sensor.data.hum == 33)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(212)));}} while(0);

    sendData((fsm_t*)(&fsm_sensor));

    UnityAssertEqualNumber((UNITY_INT)((SEND_DATA)), (UNITY_INT)((( (*(&flags_test))&(SEND_DATA) ))), (

   ((void *)0)

   ), (UNITY_UINT)(214), UNITY_DISPLAY_STYLE_INT);

    do {if ((tick == fsm_sensor.data.tickCounter)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(215)));}} while(0);



}







void test_fsm_event(void)

{



    flags_test =0;



    ( *(&flags_test) |= (MQTT_NEWDATA) );

    UnityAssertEqualNumber((UNITY_INT)((MQTT_NEWDATA)), (UNITY_INT)((checkFlagInData( (fsm_t*)(&fsm_event) ))), (

   ((void *)0)

   ), (UNITY_UINT)(227), UNITY_DISPLAY_STYLE_INT);



    ( *(&flags_test) |= (SEND_DATA) );

    UnityAssertEqualNumber((UNITY_INT)((SEND_DATA)), (UNITY_INT)((checkFlagOutData( (fsm_t*)(&fsm_event) ))), (

   ((void *)0)

   ), (UNITY_UINT)(230), UNITY_DISPLAY_STYLE_INT);





    topic_ind =TURN_LED;

    mess_size = sprintf(mess, "1");



    processData( (fsm_t*)(&fsm_event) );

    UnityAssertEqualNumber((UNITY_INT)((LED_ON)), (UNITY_INT)((( (*(&flags_test))&(LED_ON) ))), (

   ((void *)0)

   ), (UNITY_UINT)(237), UNITY_DISPLAY_STYLE_INT);



    mess_size = sprintf(mess, "0");

    processData( (fsm_t*)(&fsm_event) );

    UnityAssertEqualNumber((UNITY_INT)((LED_OFF)), (UNITY_INT)((( (*(&flags_test))&(LED_OFF) ))), (

   ((void *)0)

   ), (UNITY_UINT)(241), UNITY_DISPLAY_STYLE_INT);



    topic_ind =TURN_ALARM;

    mess_size = sprintf(mess, "1");

    processData( (fsm_t*)(&fsm_event) );

    UnityAssertEqualNumber((UNITY_INT)((ALARM_ON)), (UNITY_INT)((( (*(&flags_test))&(ALARM_ON) ))), (

   ((void *)0)

   ), (UNITY_UINT)(246), UNITY_DISPLAY_STYLE_INT);



    mess_size = sprintf(mess, "0");

    processData( (fsm_t*)(&fsm_event) );

    UnityAssertEqualNumber((UNITY_INT)((ALARM_OFF)), (UNITY_INT)((( (*(&flags_test))&(ALARM_OFF) ))), (

   ((void *)0)

   ), (UNITY_UINT)(250), UNITY_DISPLAY_STYLE_INT);





    topic_ind =COLOR_LED;

    mess_size = sprintf(mess, "5592405");

    processData( (fsm_t*)(&fsm_event) );

    UnityAssertEqualNumber((UNITY_INT)((LED_COLOR)), (UNITY_INT)((( (*(&flags_test))&(LED_COLOR) ))), (

   ((void *)0)

   ), (UNITY_UINT)(256), UNITY_DISPLAY_STYLE_INT);

    do {if ((fsm_led.data.bColor == 0x55)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(257)));}} while(0);



}







void test_fsm_control(void)

{

    flags_test =0;

    gpioVal = 0;

    tick = 0;

    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((checkConnected( (fsm_t*)(&fsm_control)))), (

   ((void *)0)

   ), (UNITY_UINT)(268), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((1)), (UNITY_INT)((checkNotConnected( (fsm_t*)(&fsm_control)))), (

   ((void *)0)

   ), (UNITY_UINT)(269), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((1)), (UNITY_INT)((checkNotConfigured( (fsm_t*)(&fsm_control)))), (

   ((void *)0)

   ), (UNITY_UINT)(270), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((checkButton( (fsm_t*)(&fsm_control)))), (

   ((void *)0)

   ), (UNITY_UINT)(271), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((checkTimerHigher( (fsm_t*)(&fsm_control)))), (

   ((void *)0)

   ), (UNITY_UINT)(272), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((1)), (UNITY_INT)((checkTimerLower( (fsm_t*)(&fsm_control)))), (

   ((void *)0)

   ), (UNITY_UINT)(273), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((1)), (UNITY_INT)((checkNotButtonTimerLower( (fsm_t*)(&fsm_control)))), (

   ((void *)0)

   ), (UNITY_UINT)(274), UNITY_DISPLAY_STYLE_INT);



    flags_test =0xFFFFFFFF;

    gpioVal = 1;

    tick = 10;

    UnityAssertEqualNumber((UNITY_INT)((1)), (UNITY_INT)((checkConnected( (fsm_t*)(&fsm_control)))), (

   ((void *)0)

   ), (UNITY_UINT)(279), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((checkNotConnected( (fsm_t*)(&fsm_control)))), (

   ((void *)0)

   ), (UNITY_UINT)(280), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((checkNotConfigured( (fsm_t*)(&fsm_control)))), (

   ((void *)0)

   ), (UNITY_UINT)(281), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((1)), (UNITY_INT)((checkButton( (fsm_t*)(&fsm_control)))), (

   ((void *)0)

   ), (UNITY_UINT)(282), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((1)), (UNITY_INT)((checkTimerHigher( (fsm_t*)(&fsm_control)))), (

   ((void *)0)

   ), (UNITY_UINT)(283), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((checkTimerLower( (fsm_t*)(&fsm_control)))), (

   ((void *)0)

   ), (UNITY_UINT)(284), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((checkNotButtonTimerLower( (fsm_t*)(&fsm_control)))), (

   ((void *)0)

   ), (UNITY_UINT)(285), UNITY_DISPLAY_STYLE_INT);



}





void test_fsm_sensor_1(void)

{

    flags_test = 0;

    fsm_sensor.data.flags = &flags_test;





    tick = 0;



    fsm_init((fsm_t*)(&fsm_sensor), sensor_fsm);

    fsm_fire((fsm_t*)(&fsm_sensor));





    do {if ((fsm_sensor.fsm.current_state == IDLE)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(302)));}} while(0);



    ( *(&flags_test) |= (START) );



    fsm_fire((fsm_t*)(&fsm_sensor));

    do {if ((fsm_sensor.fsm.current_state == WAIT)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(307)));}} while(0);





    fsm_fire((fsm_t*)(&fsm_sensor));

    do {if ((fsm_sensor.fsm.current_state == WAIT)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(311)));}} while(0);



    tick += 1000;

    fsm_fire((fsm_t*)(&fsm_sensor));

    do {if ((fsm_sensor.fsm.current_state == READ)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(315)));}} while(0);

    fsm_fire((fsm_t*)(&fsm_sensor));

    do {if ((fsm_sensor.fsm.current_state == WAIT)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(317)));}} while(0);

    UnityAssertEqualNumber((UNITY_INT)((SEND_DATA)), (UNITY_INT)((( (*(&flags_test))&(SEND_DATA) ))), (

   ((void *)0)

   ), (UNITY_UINT)(318), UNITY_DISPLAY_STYLE_INT);



    ( *(&flags_test) &= ~(START) );

    fsm_fire((fsm_t*)(&fsm_sensor));

    do {if ((fsm_sensor.fsm.current_state == IDLE)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(322)));}} while(0);



    fsm_sensor.fsm.current_state = READ;

    fsm_fire((fsm_t*)(&fsm_sensor));

    do {if ((fsm_sensor.fsm.current_state == IDLE)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(326)));}} while(0);





}





void test_fsm_led1(void)

{

    flags_test = 0;

    fsm_led.data.flags = &flags_test;





    tick = 0;



    fsm_init((fsm_t*)(&fsm_led), led_fsm);

    fsm_fire((fsm_t*)(&fsm_led));





    do {if ((fsm_led.fsm.current_state == IDLE)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(344)));}} while(0);



    ( *(&flags_test) |= (START) );



    fsm_fire((fsm_t*)(&fsm_led));

    do {if ((fsm_led.fsm.current_state == WAITING)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(349)));}} while(0);





    fsm_fire((fsm_t*)(&fsm_led));

    do {if ((fsm_led.fsm.current_state == WAITING)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(353)));}} while(0);



    ( *(&flags_test) |= (LED_ON) );



    fsm_fire((fsm_t*)(&fsm_led));

    do {if ((fsm_led.fsm.current_state == LIGHT_ON)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(358)));}} while(0);



}





void test_fsm_sensor1(void)

{

    flags_test = 0;

    fsm_event.data.flags = &flags_test;





    tick = 0;



    fsm_init((fsm_t*)(&fsm_event), eventos_fsm);

    fsm_fire((fsm_t*)(&fsm_event));





    do {if ((fsm_event.fsm.current_state == IDLE)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(375)));}} while(0);



    ( *(&flags_test) |= (START) );



    fsm_fire((fsm_t*)(&fsm_event));

    do {if ((fsm_event.fsm.current_state == COMM)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(380)));}} while(0);



    mess_size = sprintf(mess, "65353");

    topic_ind = COLOR_LED;

    ( *(&flags_test) |= (MQTT_NEWDATA) );



    fsm_fire((fsm_t*)(&fsm_event));

    do {if ((fsm_event.fsm.current_state == COMM)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(387)));}} while(0);

    do {if ((!( (*(&flags_test))&(MQTT_NEWDATA) ))) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(388)));}} while(0);

    do {if ((fsm_event.data.colorLEDData->bColor == 0x49)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(389)));}} while(0);

    do {if ((fsm_event.data.colorLEDData->gColor == 0xFF)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(390)));}} while(0);

    do {if ((fsm_event.data.colorLEDData->rColor == 0x00)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(391)));}} while(0);



    mess_size = sprintf(mess, "0");

    topic_ind = TURN_LED;

    ( *(&flags_test) |= (MQTT_NEWDATA) );



    fsm_fire((fsm_t*)(&fsm_event));

    do {if ((fsm_event.fsm.current_state == COMM)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(398)));}} while(0);

    do {if ((!( (*(&flags_test))&(MQTT_NEWDATA) ))) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(399)));}} while(0);

    do {if ((( (*(&flags_test))&(LED_OFF) ))) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(400)));}} while(0);



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
