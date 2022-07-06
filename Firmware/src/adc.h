/* adc.h Copyright (c) 2022 Khairulmizam Samsudin <xource@gmail.com
 *
 * Functions for using ADC
 *
 */

#ifndef ADC_H_
#define ADC_H_
#include "config.h"
#include <avr/io.h>
#include <util/delay.h>

#define ITEMP_GAIN      1
#define ITEMP_OFFSET    322     // Offset for 0deg Celcius
#define ITEMP_SCALE     1       // AVR temp sensor: 1LSB per celcius
#define ADC_VREF    50          // Vcc = 5.06
#define TEMP_GAIN   1085        // LMT86: 10.85mV per Celcius
#define TEMP_OFFSET 21          // Offset for 0deg Celcius


static void initADC(void) {
    // ADC clock input 50-200kHz
#ifdef ARDUINO_AVR_UNO
    // UNO use 16Mhz external oscillator
    ADCSRA |= (_BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0));
#else
    ADCSRA |= (_BV(ADPS2)| _BV(ADPS1));  // ADC clock prescaler /64
#endif
    ADCSRA |= _BV(ADEN);
}

static uint16_t getInternalTemp(void) {
    uint16_t temp;

    ADMUX = _BV(REFS1) | _BV(REFS0);    // Internal 1.1V VRef
    ADMUX |= _BV(MUX3);                 // Select ADC8 i.e. Temp sensor
    _delay_ms(10);                      // Settling time after changing ADMUX
    ADCSRA |= _BV(ADSC);                // Start conversion
    loop_until_bit_is_clear(ADCSRA, ADSC);
    temp = ITEMP_GAIN * (ADC - ITEMP_OFFSET);
    return (temp);
}

/*
 * Get VCC using internal 1.1V VRef
 */
static uint16_t getVcc100(void) {
    uint16_t vcc;

    ADMUX = _BV(REFS0);            // Vcc as Voltage Ref
    ADMUX |= _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
    _delay_ms(10);                 // Settling time after changing ADMUX
    ADCSRA |= _BV(ADSC);           // Start conversion
    loop_until_bit_is_clear(ADCSRA, ADSC);
    // vcc = 110 * 1024 / ADC;
    vcc = 112640 / ADC;
    return (vcc);
}

static uint16_t getTemp10(void) {
    uint16_t temp;
    uint16_t adc;
    uint16_t adc_vref =  getVcc100()/10;

    ADMUX = _BV(REFS0);   // Vcc as Voltage Ref
    ADMUX &= TEMPPIN;     // Select temperature pin
    _delay_ms(10);        // Settling time after changing ADMUX
    ADCSRA |= _BV(ADSC);  // Start conversion
    loop_until_bit_is_clear(ADCSRA, ADSC);
    adc = ADC;
    TRACE(3, "ADC: %d\n", adc);
    // T = ((ADC/1024)*VRef - TEMP_OFFSET ) / -TEMP_GAIN
    // T =  (TEMP_OFFSET*1024 - adc*ADC_VREF) / (TEMP_GAIN*0.01024);
    temp =  (TEMP_OFFSET*1024 - adc*adc_vref) / (TEMP_GAIN*0.01024);
    return (temp);
}

#endif  // ADC_H_
