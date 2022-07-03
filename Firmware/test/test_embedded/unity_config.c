#include "unity_config.h"
#include "config.h"
#include "usart.h"

void unityOutputStart() {
    initUSART();
}

void unityOutputChar(char c) {
//    usart_txWait();
    usart_txByte(c);
}

void unityOutputFlush() {}

void unityOutputComplete() {}
