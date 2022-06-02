#ifndef __TRACE_H__
#define __TRACE_H__

#ifdef TRACE

#ifdef Arduino_h

#include <Arduino.h>
#define TRACE_init Serial.begin(9600)
#define TRACEsize 40
char TRACEbuff[TRACEsize];
#define TRACE(fmt, ...)\
    do {    \
        snprintf(TRACEbuff, TRACEsize, "%s (%d): " fmt, __FUNCTION__, __LINE__, __VA_ARGS__); \
        Serial.println(TRACEbuff); \
    } while (0)

#else

#include <stdio.h>
#include "usart.h"
#define TRACE_init initUSART();
#define TRACEsize 40
char TRACEbuff[TRACEsize];
#define TRACE(fmt, ...)\
    do {    \
        snprintf(TRACEbuff, TRACEsize, "%s (%d): " fmt, __FUNCTION__, __LINE__, __VA_ARGS__); \
        usart_txString(TRACEbuff); \
    } while (0)

#endif //Arduino_h

#else

#define TRACE_init ""
#define TRACE(fmt, ...)


#endif //TRACE

#endif //__TRACE_H__
