#ifdef TEST

#include "unity.h"

#include "fsm_functions.h"


void setUp(void)
{
}


void tearDown(void)
{
}

void test_fsm_functions_checkStart(void)
{
    
    uint32_t flags = 0;
    fsm_generic_t fsm = {.fsm = NULL, .flags = &flags}; 

    SET_FLAGS(&flags, START);
    TEST_ASSERT(flags == START);
    
    TEST_ASSERT_EQUAL(START, checkStart((fsm_t*)(&fsm)));

    CLEAR_FLAGS(&flags, START);

    TEST_ASSERT_EQUAL(0, checkStart((fsm_t*)(&fsm)));

    SET_FLAGS(&flags, START | LED_COLOR);

    TEST_ASSERT_EQUAL(START, checkStart((fsm_t*)(&fsm)));
    TEST_ASSERT_EQUAL(LED_COLOR, checkColorChange((fsm_t*)(&fsm)));

    flags = 0xFFFFFFFF;
    TEST_ASSERT(flags&START);


    TEST_ASSERT_EQUAL(START, checkStart((fsm_t*)(&fsm)));
    TEST_ASSERT_EQUAL(LED_ON, checkOnLed((fsm_t*)(&fsm)));
    TEST_ASSERT_EQUAL(LED_OFF, checkOffLed((fsm_t*)(&fsm)));
    TEST_ASSERT_EQUAL(LED_COLOR, checkColorChange((fsm_t*)(&fsm)));
    TEST_ASSERT_EQUAL(0, checkSystemReset((fsm_t*)(&fsm)));
    CLEAR_FLAGS(&flags, START);
    TEST_ASSERT_EQUAL(1, checkSystemReset((fsm_t*)(&fsm)));








}

#endif // TEST
