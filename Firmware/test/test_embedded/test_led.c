#include "config.h"
#include <util/delay.h>
#include <unity.h>

void setUp(void) {
    LEDDDR |= 0x7;
}

void tearDown(void) {
    // clean stuff up here
}

void test_led_on(void) {
        LEDPORT |= _BV(LED1);
        LEDPORT |= _BV(LED2);
        LEDPORT |= _BV(LED3);
        TEST_ASSERT_EQUAL(LEDPORT, 0x7);
}

int main( int argc, char **argv) {
    _delay_ms(2000);

    UNITY_BEGIN();
    RUN_TEST(test_led_on);
    UNITY_END();
}
