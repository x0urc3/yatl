/* yatl.cpp Copyright (c) 2022 Khairulmizam Samsudin <xource@gmail.com
 *
 * Firmware for Yet Another Temperature Logger
 *
 * 
 */

/* Design Notes
 * - Switch pin should use AVR internal pullup
 * - Temperature sensor Vout connected 
 */
#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>

// Reserved pins    ATMega | Phy |  Uno
// Reset             PC6   |  1  |   reset
// SCK               PB5   |  19 |   13
// MISO              PB4   |  18 |   12
// MOSI              PB3   |  17 |   11
// SDA               PC4   |  27 |   A4
// SCL               PC5   |  28 |   A5
// TX                PD1   |  2  |   1
// RX                PD0   |  3  |   0

// pinout         ATMega | Phy  |  Uno
#define LEDPIN1     1   // x     |  x
#define LEDPIN2     3   // x     |  x
#define LEDPIN3     4   // x     |  x
#define GATEPIN     0   // x     |  x
#define TEMPPIN     PC0 //  23   |  A0
#define SWITCHPIN   2   // x     |  x

#define TEMP_GAIN	    1
#define TEMP_OFFSET	    410     // Biased at 2V. 2/5*1024=410
#define TEMP_SCALE      500     // LM35: 10mV per Celcius i.e. 5V/0.010 = 500
#define VRef            1.1                 
#define ITEMP_GAIN	    1
#define ITEMP_OFFSET    322     // Offset for 0deg Celcius	
#define ITEMP_SCALE     1       // AVR temp sensor: 1LSB per celcius    

static inline void initADC(void) {
    //ADC clock input 50-200kHz
    
#ifdef ARDUINO_AVR_UNO
    //Use UNO default
#else
    ADCSRA |= (_BV(ADPS2)| _BV(ADPS1)); // ADC clock prescaler /64
    //ADCSRA |= (_BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0)); // ADC clock prescaler /128
#endif 
}

uint16_t getTemp(void) { 
    uint16_t temp;

    ADCSRA |= (1 << ADEN);                      // enable ADC
    ADMUX &= (~(1 << REFS1) | ~(1 << REFS0));   // Vcc as Voltage Ref 
    ADMUX &= 0xf0;                              // Select PB5 (ADC0)
    ADCSRA |= (1 << ADSC);                      // Start conversion
    loop_until_bit_is_clear(ADCSRA, ADSC);
    temp = TEMP_GAIN * (ADC - TEMP_OFFSET) * TEMP_SCALE / 1024; 
    return (temp); 
}

uint16_t getInternalTemp(void) { 
    uint16_t temp;

    ADMUX = _BV(REFS1) | _BV(REFS0);    // Internal 1.1V Voltage Ref 
    ADMUX |= _BV(MUX3);                 // Select ADC8 i.e. Temp sensor
    ADCSRA |= _BV(ADEN);                // enable ADC
    ADCSRA |= _BV(ADSC);                // Start conversion
    loop_until_bit_is_clear(ADCSRA, ADSC);
    temp = ITEMP_GAIN * (ADC - ITEMP_OFFSET);
    return (temp);           
}

void setup() {
    initADC();

    pinMode(LEDPIN1, OUTPUT);
    pinMode(LEDPIN2, OUTPUT);
    pinMode(LEDPIN3, OUTPUT);
    pinMode(GATEPIN, OUTPUT);
    pinMode(TEMPPIN, INPUT);
    pinMode(SWITCHPIN, INPUT_PULLUP);

    Serial.begin(9600);
    //	mySerial.begin(9600);
    //    OSCCAL=200;
}

void loop() {
    char buff[20];
    sprintf(buff, "Internal temp: %d", getInternalTemp());
    Serial.println(buff);
    _delay_ms(1000);

}
