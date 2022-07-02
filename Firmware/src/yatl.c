/* yatl.cpp Copyright (c) 2022 Khairulmizam Samsudin <xource@gmail.com
 *
 * Firmware for Yet Another Temperature Logger
 *
 */

#include "config.h"
#include "trace.h"
#include "adc.h"
#include "interrupt.h"
#include "eeprom.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>

enum {sleep, battery, storage, logging} state;
#define STATEMAX 4

#define TIMEOUTT1_MS 5000
#define resetCounterT1() (TCNT1 = 0)
#define expiredCounterT1() ((TCNT1 > TIMEOUTT1_MS) ? 1 : 0)

#ifdef ARDUINO_AVR_ATmega48
#define TIMEOUTWDT  TIMEOUTWDT_2m
#else
#define TIMEOUTWDT  TIMEOUTWDT_1m
#endif

static void initCounterT1(void) {
    TCCR1B = _BV(CS12) | _BV(CS10);     // 1Mhz/1024 = 976Hz ~ 1000Hz
}

static void initPin(void) {
    SWITCH_PORT |= _BV(SWITCH);         // Enable pullup for switch
}

static uint8_t debounce(void) {
    //    TRACE(3,"switch:%d\n", SWITCH_PIN & _BV(SWITCH));
    if (bit_is_clear(SWITCH_PIN, SWITCH)) {      // switch pressed
        _delay_us(SWITCH_DEBOUNCE_TIME);
        if (bit_is_clear(SWITCH_PIN, SWITCH)) {  // still pressed
            return (1);
        }
    }
    return 0;
}

static void doSleep() {
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sei();
    sleep_mode();
    cli();
}

static void setup(void) {
    initADC();
    initEEPROM();
    initPin();
    initCounterT1();
    initInterrupt();
    initWDT();
}

int main(void) {
    TRACE_init();
    setup();

    uint8_t switchClicked = 0;
    uint8_t clickCount = sleep;
    uint8_t logState = 0;
    uint8_t countWDT = 0;

    for (;;) {
        if (debounce() && !(g_flagWDT)) {
            if (switchClicked == 0) {
                clickCount = (clickCount+1) % STATEMAX;     // Rollover counter
                if (clickCount == 0) {
                    resetCounterT1();
                }
                switchClicked = 1;
                TRACE(1, "click:%d\n", clickCount);
            }
        } else {
            switchClicked = 0;
        }

        if (expiredCounterT1() && !(g_flagWDT)) {
            TRACE(1, "Timeout. click:%d\n", clickCount);

            if (clickCount == battery) {
                TRACE(1, "Show battery. VCC10:%d\n", getVcc100()/10);
            }
            if (clickCount == storage) {
                uint16_t usage = usageEEPROM();
                TRACE(1, "Show storage. romDataPnt:%d\n", usage);
            }
            if (clickCount == logging) {
                if (!logState) {
                    logState = 1;
                    startWDT();
                } else {
                    logState = 0;
                    stopWDT();
                }
                TRACE(1, "logstate:%d\n", logState);
            }
            if (clickCount == sleep) {
                TRACE(1, "Going to sleep. logState:%d\n", logState);
                doSleep();
                TRACE(1, "Wakeup from sleep. logState:%d\n", logState);
            }
            clickCount = 0;
            resetCounterT1();
        }

        if ((logState && g_flagWDT)) {
            if ( countWDT == TIMEOUTWDT ) {
                uint16_t temp = getTemp10();
                TRACE(1, "Do logging. temp:%d\n", temp);
                temp = temp - 200;      // Scale down to fit byte
                uint8_t error = storeByteEEPROM(temp);
                if (error != 0) {
                    TRACE(1, "Storage full. error:%d\n", error);
                    logState = 0;
                    stopWDT();
                }
                countWDT = 0;
            } else {
                countWDT += 1;
            }
            g_flagWDT = 0;
        }
    }
    return(0);
}
