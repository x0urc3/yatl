#include <avr/io.h>
#include <util/delay.h>
#include "trace.h"

#define LED1        PB0   //       |  x     |  x
#define LED2        PB1   //       |  x     |  x
#define LED3        PB2   //       |  x     |  x

int main(void) {
    TRACE_init();
    TRACE(1,"Initialize board %d\n",sizeof(int));
    DDRB |= 0x7;

    int val = 0;
    while (1) {
        TRACE(1,"Shift right. Button press:%d\n",val);
        for (val = 0x1; val != 0x8; val <<= 1){
            PORTB = val;
            _delay_ms(1000);
        }
        TRACE(1,"Shift left. Button press:%d\n",val);
        for (val = 0x04; val != 0x0; val >>= 1) {
            PORTB = val;
            _delay_ms(1000);
        }
    }

    return(0);
}
