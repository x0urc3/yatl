#ifndef __DEBUG_H__
#define __DEBUG_H__

#ifdef DEBUG

#include <Arduino.h>
#define TRACE_init Serial.begin(9600)
char TRACEbuff[40];
#define TRACE(fmt, ...)\
    do {    \
        sprintf(TRACEbuff,"%s (%d): " fmt, __FUNCTION__, __LINE__, __VA_ARGS__); \
        Serial.println(TRACEbuff); \
    } while (0)

#else

#define TRACE_init ""
#define TRACE(fmt, ...)


#endif //DEBUG
#endif //__DEBUG_H__
