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
// ARef (100nF Gnd)  ARef  |  20 |   ARef

// pinout                   ATMega  | Phy  |  Uno
#define LEDPIN1     1   //          |  x     |  x
#define LEDPIN2     3   //          |  x     |  x
#define LEDPIN3     4   //          |  x     |  x
#define GATEPIN     0   //          |  x     |  x
#define TEMPPIN     0xf0//  ADC0    |   23   |  A0
#define SWITCHPIN   2   // x     |  x

#define ITEMP_GAIN	    1
#define ITEMP_OFFSET    322     // Offset for 0deg Celcius	
#define ITEMP_SCALE     1       // AVR temp sensor: 1LSB per celcius    
#define ADC_VREF    50    // Vcc = 5.06
#define TEMP_GAIN   1085  // LMT86: 10.85mV per Celcius 
#define TEMP_OFFSET	21    // Offset for 0deg Celcius

static inline void initADC(void) {
    //ADC clock input 50-200kHz
    
#ifdef ARDUINO_AVR_UNO
    //UNO use 16Mhz external oscillator
    ADCSRA |= (_BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0)); // ADC clock prescaler /128
#else
    ADCSRA |= (_BV(ADPS2)| _BV(ADPS1)); // ADC clock prescaler /64
#endif 
    ADCSRA |= _BV(ADEN);                // enable ADC
}


uint16_t getTemp(void) { 
    uint16_t temp;

    ADMUX = _BV(REFS0);   // Vcc as Voltage Ref 
    ADMUX &= TEMPPIN;     // Select temperature pin
    ADCSRA |= _BV(ADSC);  // Start conversion
    loop_until_bit_is_clear(ADCSRA, ADSC);
    //T = ((ADC/1024)*VRef - TEMP_OFFSET ) / -TEMP_GAIN
    temp =  (TEMP_OFFSET*1024 - ADC*ADC_VREF)/ (TEMP_GAIN*0.001024); 
    return (temp); 
}

uint16_t getInternalTemp(void) { 
    uint16_t temp;

    ADMUX = _BV(REFS1) | _BV(REFS0);    // Internal 1.1V Voltage Ref 
    ADMUX |= _BV(MUX3);                 // Select ADC8 i.e. Temp sensor
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
