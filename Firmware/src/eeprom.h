#ifndef EEPROM_H__
#define EEPROM_H__
#include <avr/eeprom.h>

#define ROM_DIRTYFLAG 0x7a
#ifdef ARDUINO_AVR_ATmega48
#define ROM_SIZE 250
#else
#define ROM_SIZE 500
#endif

struct eeDataStruct {
    uint8_t dirty;
    uint16_t dataPnt;
    uint8_t data[ROM_SIZE];
};

struct eeDataStruct EEMEM eeData = {0};     // Required to be defined

#define usageEEPROM() (eeprom_read_word(&eeData.dataPnt))

void initEEPROM(void) {
    uint8_t flag;
    uint16_t addr;

    TRACE(1, "Reading flag. DIRTY:%d\n", flag);
    TRACE(1, "DIRTYFLAG:%d\n", ROM_DIRTYFLAG);
    flag = eeprom_read_byte(&eeData.dirty);
    if (flag != ROM_DIRTYFLAG) {
        TRACE(1, "ROM is empty. DIRTY:%d\n", flag);
        addr = eeprom_read_word(&eeData.dataPnt);
        if (addr != 0) {
            TRACE(1, "Reset addr. addr:%d\n", addr);
            eeprom_write_word(&eeData.dataPnt, 0);
        }
    }
}

uint8_t storeByteEEPROM(uint8_t data) {
    uint16_t addr;
    addr = eeprom_read_word(&eeData.dataPnt);
    TRACE(1, "Current addr:%d\n", addr);

    if (addr == 0) {
        eeprom_write_byte(&eeData.dirty, ROM_DIRTYFLAG);
    }

    if (addr < ROM_SIZE) {
        TRACE(1, "Storing data:%d\n", data);
        TRACE(1, "Storing at addr:%d\n", addr);
        eeprom_update_byte(&eeData.data[addr], data);

        addr += 1;
        eeprom_write_word(&eeData.dataPnt, addr);

        return 0;
    }
    return 1;
}

#endif  // EEPROM_H__
