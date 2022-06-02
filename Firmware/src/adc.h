#ifndef __ADC_H__
#define __ADC_H__
#include <avr/io.h>
#include <util/delay.h>

#define ITEMP_GAIN	    1
#define ITEMP_OFFSET    322     // Offset for 0deg Celcius	
#define ITEMP_SCALE     1       // AVR temp sensor: 1LSB per celcius    

void initADC(void) {
    //ADC clock input 50-200kHz
    
#ifdef ARDUINO_AVR_UNO
    //UNO use 16Mhz external oscillator
    ADCSRA |= (_BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0)); // ADC clock prescaler /128
#else
    ADCSRA |= (_BV(ADPS2)| _BV(ADPS1)); // ADC clock prescaler /64
#endif 
    ADCSRA |= _BV(ADEN);                // enable ADC
}

uint16_t getInternalTemp(void) { 
    uint16_t temp;

    ADMUX = _BV(REFS1) | _BV(REFS0);    // Internal 1.1V VRef
    ADMUX |= _BV(MUX3);                 // Select ADC8 i.e. Temp sensor
    _delay_ms(10);                      // Settling time after changing ADMUX
    ADCSRA |= _BV(ADSC);                // Start conversion
    loop_until_bit_is_clear(ADCSRA, ADSC);
    temp = ITEMP_GAIN * (ADC - ITEMP_OFFSET);
    return (temp);           
}

uint16_t getVcc100(void) {
    uint16_t vcc;

    ADMUX = _BV(REFS0);            // Vcc as Voltage Ref
    ADMUX |= _BV(MUX3) | _BV(MUX2) |
        _BV(MUX1);                 // Select internal 1.1V VRef
    _delay_ms(10);                  // Settling time after changing ADMUX
    ADCSRA |= _BV(ADSC);           // Start conversion
    loop_until_bit_is_clear(ADCSRA, ADSC);
    //vcc = 110 * 1024 / ADC;
    vcc = 112640 / ADC;
    return (vcc);
}

#endif //__ADC_H__
