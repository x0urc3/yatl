/* interrupt.h Copyright (c) 2022 Khairulmizam Samsudin <xource@gmail.com
 *
 * Interrupt functions
 *
 */

#ifndef INTERRUPT_H__
#define INTERRUPT_H__
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#define TIMEOUTWDT_1m   8       // 60s/8s = 7.5 ~ 8
#define TIMEOUTWDT_2m   16
volatile uint8_t g_flagWDT = 0;

static void initInterrupt(void) {
  PCICR |= (1 << PCIE2);        // Set pin-change interrupt for D pins
  PCMSK2 |= (1 << PCINT18);     // Set mask to for PD2/PCINT18
}

EMPTY_INTERRUPT(PCINT2_vect);

void initWDT() {
    cli();
    MCUSR &= ~_BV(WDRF);

//    WDTCSR = _BV(WDIF) | _BV(WDIE) | WDTO_8S| _BV(WDCE) | _BV(WDE);
//    WDTCSR = _BV(WDIF) | _BV(WDIE) | WDTO_8S;   //Clear WDCE and WDE
    WDTCSR = 0xf9;
//    WDTCSR = 0xe1;   //Clear WDCE and WDE
    WDTCSR = 0xa1;   //Clear WDIE, WDCE and WDE
    TRACE(1,"WDTCSR:%X",WDTCSR);
}
#define startWDT()  (WDTCSR |= _BV(WDIE))
#define stopWDT()   (WDTCSR &= ~_BV(WDIE))

ISR(WDT_vect) {
    g_flagWDT = 1;
}

#endif  // INTERRUPT_H__
