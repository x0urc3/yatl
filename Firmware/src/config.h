/* config.h Copyright (c) 2022 Khairulmizam Samsudin <xource@gmail.com
 *
 * Pins and configuration declaration
 *
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <avr/io.h>

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

// pinout         ATMega    | Phy  |  Uno
#define LED1        PB0   //       |  x     |  x
#define LED2        PB1   //       |  x     |  x
#define LED3        PB2   //       |  x     |  x
#define LEDDDR      DDRB
#define LEDPORT     PORTB
#define TEMPPIN     0xf0  //   23  |  A0
#define SWITCH      PD2   //       |  x     |  x
#define SWITCH_PIN  PIND
#define SWITCH_PORT PORTD
#define SWITCH_DEBOUNCE_TIME 100

#endif  // CONFIG_H_

