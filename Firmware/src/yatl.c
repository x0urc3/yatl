/* yatl.cpp Copyright (c) 2022 Khairulmizam Samsudin <xource@gmail.com
 *
 * Firmware for Yet Another Temperature Logger
 *
 */

#include "adc.h"
#include "trace.h"
#include "usart.h"
#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

// Reserved pins    ATMega | Phy |  Uno
// Reset             PC6   |  1  |   reset
// SCK               PB5   |  19 |   13
// MISO              PB4   |  18 |   12
// MOSI              PB3   |  17 |   11
// SDA               PC4   |  27 |   A4
// SCL               PC5   |  28 |   A5
// TX                PD1   |  2  |   1
// RX                PD0   |  3  |   0
// ARef (100nF Gnd)  ARef  |  20 |   ARef

// pinout         ATMega    | Phy  |  Uno
#define LED1        PB0   //       |  x     |  x
#define LED2        PB1   //       |  x     |  x
#define LED3        PB2   //       |  x     |  x
#define LEDPIN      PINB
#define LEDPORT     PORTB
#define TEMPPIN     0xf0  //   23  |  A0
#define SWITCH      PD2   //       |  x     |  x
#define SWITCH_PIN  PIND
#define SWITCH_PORT PORTD
#define SWITCH_DEBOUNCE_TIME 100

#define ADC_VREF    50    // Vcc = 5.06
#define TEMP_GAIN   1085  // LMT86: 10.85mV per Celcius
#define TEMP_OFFSET	21    // Offset for 0deg Celcius

#define ROM_DIRTY 0x7a
#define ROM_SIZE 500
uint8_t EEMEM romDirty;
uint16_t EEMEM romCnt;
uint8_t EEMEM romData[ROM_SIZE];



#define TIMEOUT_MS 5000
#define resetCounterT1() (TCNT1 = 0)
#define expiredCounterT1() ((TCNT1 > TIMEOUT_MS) ? 1 : 0)

#define PCINT 1
#define WDTINT 2
volatile uint8_t g_interruptStatus;

#define SLEEP     0
#define BATTERY   1
#define STORAGE   2
#define LOGGING   3
#define STATE_SLEEP     (_BV(SLEEP))
#define STATE_BATTERY   (_BV(BATTERY))
#define STATE_STORAGE   (_BV(STORAGE))
#define STATE_LOGGING   (_BV(LOGGING))
#define STATEMAX        4

static void initInterrupt(void) {
  PCICR |= (1 << PCIE2);        // Set pin-change interrupt for D pins
  PCMSK2 |= (1 << PCINT18);     // Set mask to for PD2/PCINT18
}

ISR(PCINT2_vect) {
    g_interruptStatus = PCINT;
}

static void initCounterT1(void) {
    TCCR1B = _BV(CS12) | _BV(CS10); //1Mhz/1024 = 976Hz ~ 1000Hz
}

static void initPin(void) {
    SWITCH_PORT |= _BV(SWITCH);     //Enable pullup for switch
}

static void initEEPROM(void) {
    uint8_t tt;
    tt = eeprom_read_byte(&romDirty);
    if (tt != ROM_DIRTY) {
        TRACE(1, "Reset counter. DIRTY:%d\n",tt);
        tt = eeprom_read_word(&romCnt);
        if (tt != 0) {
            TRACE(1, "Reset counter. CNT:%d\n",tt);
            eeprom_update_word(&romCnt,0);
        }
    }
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

static uint16_t getTemp10(void) {
    uint16_t temp;
    uint16_t adc_vref;
    uint16_t adc;

    ADMUX = _BV(REFS0);   // Vcc as Voltage Ref
    ADMUX &= TEMPPIN;     // Select temperature pin
    _delay_ms(10);        // Settling time after changing ADMUX
    ADCSRA |= _BV(ADSC);  // Start conversion
    loop_until_bit_is_clear(ADCSRA, ADSC);
    adc = ADC;
    //TRACE(3,"ADC: %d\n", adc);
    //T = ((ADC/1024)*VRef - TEMP_OFFSET ) / -TEMP_GAIN
    //temp =  (TEMP_OFFSET*1024 - adc*ADC_VREF)/ (TEMP_GAIN*0.01024);
    adc_vref =  getVcc100()/10;
    temp =  (TEMP_OFFSET*1024 - adc*adc_vref) / (TEMP_GAIN*0.01024);
    return (temp);
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
    TRACE_init();
}



int main(void) {
    setup();

    uint8_t switchClicked = 0;
    uint8_t clickCount = SLEEP;
    uint8_t currentState = STATE_SLEEP;  //Use to track ONE or MORE state

    for (;;) {
        //TRACE("Internal temp: %d", getInternalTemp());
        //TRACE("test %d\n", 5);
        if (debounce()) {
            if (switchClicked == 0) {
                clickCount += 1;
                switchClicked = 1;
                TRACE(3,"click:%d\n", clickCount);
            }
        } else {
            switchClicked = 0;
        }

        if (expiredCounterT1()) {
            resetCounterT1();
            currentState |= (clickCount > STATEMAX) ? _BV(STATEMAX) : _BV(clickCount);
            TRACE(1,"Timeout. click:%d\n", clickCount);
            TRACE(1,"Timeout. currentState:%d\n", currentState);
            clickCount = 0;

            if (currentState & STATE_BATTERY) {
                TRACE(1,"Show battery. VCC10:%d\n", getVcc100()/10);
                currentState &= ~STATE_BATTERY;
            }
            if (currentState & STATE_STORAGE) {
                TRACE(1,"Show storage. romCnt:%d\n", romCnt);
                currentState &= ~STATE_STORAGE;
            }
            if (currentState & STATE_LOGGING) {
                TRACE(1,"Do logging. state:%d\n", currentState);
            }
            if (currentState & STATE_SLEEP) {
                TRACE(1,"Going to sleep. state:%d\n", currentState);
                doSleep();
                TRACE(1,"Wakeup from sleep. state:%d\n", currentState);
//                _delay_ms(1000);
            }
        }

    }
    return(0);
}
