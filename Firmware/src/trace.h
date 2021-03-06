/* trace.h Copyright (c) 2022 Khairulmizam Samsudin <xource@gmail.com
 *
 * Tracing functions for debugging purposes
 *
 */

#ifndef TRACE_H__
#define TRACE_H__

#ifdef TRACER

#ifndef TRACEVERBOSE
#warning "TRACEVERBOSE default to 1"
#define TRACEVERBOSE 1
#endif

#include <stdio.h>
#include "usart.h"
#define TRACE_init() (initUSART())
#define TRACEsize 40
char TRACEbuff[TRACEsize];
#define TRACE(verbose, fmt, ...)\
    do {    \
        if (verbose <= TRACEVERBOSE) { \
            snprintf(TRACEbuff, TRACEsize, "%s (%d): " fmt, __FUNCTION__, __LINE__, __VA_ARGS__); \
            usart_txString(TRACEbuff); \
        } \
    } while (0)

#else

#define TRACE_init()
#define TRACE(fmt, ...)

#endif  // TRACE

#endif  // TRACE_H__
