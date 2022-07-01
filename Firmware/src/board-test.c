#include <avr/io.h>
#include <util/delay.h>
#include "trace.h"

//#define LED_PIN PB2
#define LED_PIN PB0
int main(void) {
    //setup
    TRACE_init();
//    DDRB |= (1 << LED_PIN);
    DDRB |= _BV(LED_PIN);

    //loop
    while (1) {
        PORTB ^= _BV(LED_PIN);
        TRACE(1,"Test serial %d",1);
        _delay_ms(1000);
    }

    return(0);
}
