/* interrupt.h Copyright (c) 2022 Khairulmizam Samsudin <xource@gmail.com
 *
 * Interrupt functions
 *
 */

#ifndef INTERRUPT_H__
#define INTERRUPT_H__
#include <avr/io.h>
#include <avr/interrupt.h>

#define TIMEOUTWDT_1m   8       // 60s/8s = 7.5 ~ 8
#define TIMEOUTWDT_2m   16
volatile uint8_t g_flagWDT = 0;

static void initInterrupt(void) {
  PCICR |= (1 << PCIE2);        // Set pin-change interrupt for D pins
  PCMSK2 |= (1 << PCINT18);     // Set mask to for PD2/PCINT18
}

EMPTY_INTERRUPT(PCINT2_vect);

void initWDT() {
    WDTCSR = _BV(WDCE);
    WDTCSR |= _BV(WDP3) | _BV(WDP0);    // ~8s timeout
}
#define startWDT()  (WDTCSR |= _BV(WDIE))
#define stopWDT()   (WDTCSR &= ~_BV(WDIE))

ISR(WDT_vect) {
    g_flagWDT = 1;
}

#endif  // INTERRUPT_H__
