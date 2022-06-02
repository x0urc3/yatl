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
#include "adc.h"
#include "trace.h"
#include "usart.h"
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
#define LEDPIN1     PB0   //          |  x     |  x
#define LEDPIN2     PB1   //          |  x     |  x
#define LEDPIN3     PB2   //          |  x     |  x
#define SWITCHPIN   PD2   // x     |  x
#define TEMPPIN     0xf0  // ADC0    |   23   |  A0


#define ADC_VREF    50    // Vcc = 5.06
#define TEMP_GAIN   1085  // LMT86: 10.85mV per Celcius 
#define TEMP_OFFSET	21    // Offset for 0deg Celcius

uint16_t getTemp10(void) {
    uint16_t temp;

    ADMUX = _BV(REFS0);   // Vcc as Voltage Ref 
    ADMUX &= TEMPPIN;     // Select temperature pin
    _delay_ms(2);         // Settling time after changing ADMUX
    ADCSRA |= _BV(ADSC);  // Start conversion
    loop_until_bit_is_clear(ADCSRA, ADSC);
    //T = ((ADC/1024)*VRef - TEMP_OFFSET ) / -TEMP_GAIN
    temp =  (TEMP_OFFSET*1024 - ADC*ADC_VREF)/ (TEMP_GAIN*0.01024);
    return (temp); 
}

void setup(void) {
    initADC();
/*
    pinMode(LEDPIN1, OUTPUT);
    pinMode(LEDPIN2, OUTPUT);
    pinMode(LEDPIN3, OUTPUT);
    pinMode(GATEPIN, OUTPUT);
    pinMode(TEMPPIN, INPUT);
    pinMode(SWITCHPIN, INPUT_PULLUP);
*/
    TRACE_init;
    //Serial.begin(9600);
    //	mySerial.begin(9600);
    //    OSCCAL=200;

    //initUSART();
}

uint8_t d = 'u';
void loop(void) {
    //TRACE("Internal temp: %d", getInternalTemp());
    TRACE("test %d\n", 5);
    //Serial.println("test");
    /*
    if (usart_txReady()) {
        usart_txByte(d);
    }
    usart_rxWait();
    d = usart_rxByte();
    char aa[]="Ayam\n";
    usart_txNByte(aa,5);
    */
    _delay_ms(1000);

}

#ifndef Arduino_h
int main(void) {
    setup();
    for (;;) {
        loop();
        //serial event here
    }
    return(0);
}
#endif
