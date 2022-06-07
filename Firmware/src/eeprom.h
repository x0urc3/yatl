#ifndef EEPROM_H__
#define EEPROM_H__
#include <avr/eeprom.h>

#define ROM_DIRTYFLAG 0x7a
#define ROM_SIZE 500
uint8_t EEMEM romDirty;
uint16_t EEMEM romCnt;
uint8_t EEMEM romData[ROM_SIZE];

static void initEEPROM(void) {
    uint8_t flag;
    uint16_t counter;
    flag = eeprom_read_byte(&romDirty);
    if (flag != ROM_DIRTYFLAG) {
        TRACE(3, "Reset counter. DIRTY:%d\n", flag);
        counter = eeprom_read_word(&romCnt);
        if (counter != 0) {
            TRACE(3, "Reset counter. CNT:%d\n", counter);
            eeprom_update_word(&romCnt, 0);
        }
    }
}

#endif  // EEPROM_H__
