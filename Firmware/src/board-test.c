#include "config.h"
#include "trace.h"
#include "adc.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile int shiftL = 0;
ISR(INT0_vect) {
    shiftL ^= 0xff;
}

void setup(void) {
    TRACE_init();
    TRACE(1,"Initialize board %d\n",sizeof(int));

    DDRB |= 0x7;                    // LED output

    SWITCH_PORT |= _BV(SWITCH);     // Enable pullup for switch
    EIMSK = _BV(INT0);              // Switch is tied to INT0
    EICRA = _BV(ISC01);             // Interrupt on falling edge
    sei();

    initADC();
}

int main(void) {
    setup();

    int val = 0;
    int temp = 0;
    while (1) {
        if (shiftL == 0) {
            TRACE(1,"Shift right. shiftL:%d\n", shiftL);
            for (val = 0x1; val != 0x8; val <<= 1){
                PORTB = val;
                _delay_ms(1000);
            }
        }
        if (shiftL == 255) {
            TRACE(1,"Shift left. shiftL:%d\n", shiftL);
            for (val = 0x04; val != 0x0; val >>= 1) {
                PORTB = val;
                _delay_ms(1000);
            }
        }

        temp = getTemp10();
        TRACE(1, "getTemp10():%d\n", temp);
    }

    return(0);
}
