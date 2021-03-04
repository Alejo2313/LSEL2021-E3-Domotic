#include "include/fsm_functions.h"
#include "/var/lib/gems/2.7.0/gems/ceedling-0.31.0/vendor/unity/src/unity.h"














void setUp(void)

{

}





void tearDown(void)

{

}



void test_fsm_functions_checkStart(void)

{



    uint32_t flags = 0;

    fsm_generic_t fsm = {.fsm = 

                               ((void *)0)

                                   , .flags = &flags};



    ( *(&flags) |= (START) );

    do {if ((flags == START)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(26)));}} while(0);



    UnityAssertEqualNumber((UNITY_INT)((START)), (UNITY_INT)((checkStart((fsm_t*)(&fsm)))), (

   ((void *)0)

   ), (UNITY_UINT)(28), UNITY_DISPLAY_STYLE_INT);



    ( *(&flags) &= ~(START) );



    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((checkStart((fsm_t*)(&fsm)))), (

   ((void *)0)

   ), (UNITY_UINT)(32), UNITY_DISPLAY_STYLE_INT);



    ( *(&flags) |= (START | LED_COLOR) );



    UnityAssertEqualNumber((UNITY_INT)((START)), (UNITY_INT)((checkStart((fsm_t*)(&fsm)))), (

   ((void *)0)

   ), (UNITY_UINT)(36), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((LED_COLOR)), (UNITY_INT)((checkColorChange((fsm_t*)(&fsm)))), (

   ((void *)0)

   ), (UNITY_UINT)(37), UNITY_DISPLAY_STYLE_INT);



    flags = 0xFFFFFFFF;

    do {if ((flags&START)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(40)));}} while(0);





    UnityAssertEqualNumber((UNITY_INT)((START)), (UNITY_INT)((checkStart((fsm_t*)(&fsm)))), (

   ((void *)0)

   ), (UNITY_UINT)(43), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((LED_ON)), (UNITY_INT)((checkOnLed((fsm_t*)(&fsm)))), (

   ((void *)0)

   ), (UNITY_UINT)(44), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((LED_OFF)), (UNITY_INT)((checkOffLed((fsm_t*)(&fsm)))), (

   ((void *)0)

   ), (UNITY_UINT)(45), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((LED_COLOR)), (UNITY_INT)((checkColorChange((fsm_t*)(&fsm)))), (

   ((void *)0)

   ), (UNITY_UINT)(46), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((checkSystemReset((fsm_t*)(&fsm)))), (

   ((void *)0)

   ), (UNITY_UINT)(47), UNITY_DISPLAY_STYLE_INT);

    ( *(&flags) &= ~(START) );

    UnityAssertEqualNumber((UNITY_INT)((1)), (UNITY_INT)((checkSystemReset((fsm_t*)(&fsm)))), (

   ((void *)0)

   ), (UNITY_UINT)(49), UNITY_DISPLAY_STYLE_INT);

}
