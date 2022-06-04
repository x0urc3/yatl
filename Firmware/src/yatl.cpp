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
#include <avr/eeprom.h>
#include <util/delay.h>
#include <avr/sleep.h>

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

#define ROM_DIRTY 0x7a
#define ROM_SIZE 500
uint8_t EEMEM rom_dirty;
uint16_t EEMEM rom_cnt;
uint8_t EEMEM rom_data[ROM_SIZE];

void initEEPROM(void) {
    uint8_t tt;
    tt = eeprom_read_byte(&rom_dirty);
    if (tt != ROM_DIRTY) {
        TRACE("Reset counter. DIRTY:%d\n",tt);
        tt = eeprom_read_word(&rom_cnt);
        if (tt != 0) {
            TRACE("Reset counter. CNT:%d\n",tt);
            eeprom_update_word(&rom_cnt,0);
        }
    }
}

uint16_t getTemp10(void) {
    uint16_t temp;
    uint16_t adc_vref;
    uint16_t adc;

    ADMUX = _BV(REFS0);   // Vcc as Voltage Ref 
    ADMUX &= TEMPPIN;     // Select temperature pin
    _delay_ms(10);         // Settling time after changing ADMUX
    ADCSRA |= _BV(ADSC);  // Start conversion
    loop_until_bit_is_clear(ADCSRA, ADSC);
    adc = ADC;
    TRACE("ADC: %d\n", adc);
    //T = ((ADC/1024)*VRef - TEMP_OFFSET ) / -TEMP_GAIN
    //temp =  (TEMP_OFFSET*1024 - adc*ADC_VREF)/ (TEMP_GAIN*0.01024);
    adc_vref =  getVcc100()/10;
    temp =  (TEMP_OFFSET*1024 - adc*adc_vref) / (TEMP_GAIN*0.01024);
    return (temp); 
}

void doSleep() {
 //cbi(ADCSRA,ADEN); // Switch Analog to Digital converter OFF

 set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set sleep mode
 sleep_mode(); // System sleeps here
 //sbi(ADCSRA,ADEN);  // Switch Analog to Digital converter ON
}

void setup(void) {
    initADC();
    initEEPROM();
    TRACE_init;
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
