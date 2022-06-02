/*
 * usart.h - Functions to initialize, send, receive over USART
 */
#ifndef __USART_H__
#define __USART_H__

#include <avr/io.h>
#ifndef BAUD                          
#define BAUD  9600   //Set a safe default baud rate
#endif
#include <util/setbaud.h>     //Calculate bit-clock multiplier based on BAUD and F_CPU

#define usart_txReady()       (bit_is_set(UCSR0A, UDRE0))
#define usart_rxReady()       (bit_is_set(UCSR0A, RXC0))
#define usart_txWait()        ({loop_until_bit_is_set(UCSR0A, UDRE0);})
#define usart_rxWait()        ({loop_until_bit_is_set(UCSR0A, RXC0);})
#define usart_txByte(data)    (UDR0 = data)
#define usart_rxByte()        (UDR0)

void initUSART(void) {
  UBRR0H = UBRRH_VALUE;         // defined in setbaud.h & requires BAUD
  UBRR0L = UBRRL_VALUE;
#if USE_2X
  UCSR0A |= _BV(U2X0);
#else
  UCSR0A &= ~_BV(U2X0);
#endif

  UCSR0B = _BV(TXEN0) | _BV(RXEN0);     //Enable USART transmitter/receiver
  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);   // 8 data bits, 1 stop bit
}

void usart_txString(char data[]) {
  uint8_t i = 0;
  while (data[i]) {
      usart_txWait();
      usart_txByte(data[i]);
      i++;
  }
}

void usart_txNByte(char data[], int n) {
  uint8_t i = 0;
  do {
      usart_txWait();
      usart_txByte(data[i]);
      i++;
  } while (i < n);
}

#endif // __USART_H__
