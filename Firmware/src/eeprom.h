#ifndef EEPROM_H__
#define EEPROM_H__
#include <avr/eeprom.h>

#define ROM_DIRTYFLAG 0x7a
#ifdef ARDUINO_AVR_ATmega48
#define ROM_SIZE 250
#else
#define ROM_SIZE 500
#endif

uint8_t EEMEM romDirty;
uint16_t romDataPnt EEMEM;
uint8_t romData[ROM_SIZE] EEMEM;

static void initEEPROM(void) {
    uint8_t flag;
    uint16_t addr;
    flag = eeprom_read_byte(&romDirty);
    addr = eeprom_read_word(&romDataPnt);
//    eeprom_read_byte(&romData[0]);           //prevent compiler optimization

    TRACE(1, "Reading flag. DIRTY:%d\n", flag);
    TRACE(1, "DIRTYFLAG:%d\n", ROM_DIRTYFLAG);
    if (flag != ROM_DIRTYFLAG) {
        TRACE(1, "ROM is empty. DIRTY:%d\n", flag);
        if (addr != 0) {
            TRACE(1, "Reset addr. addr:%d\n", addr);
            eeprom_write_word(&romDataPnt, 0);
        }
    }
}

uint8_t storeByteEEPROM(uint8_t data) {
    uint16_t addr;
    addr = eeprom_read_word(&romDataPnt);
    TRACE(1, "Current addr:%d\n", addr);

    if (addr == 0) {
        eeprom_write_byte(&romDirty, ROM_DIRTYFLAG);
    }

    if (addr < ROM_SIZE) {
        TRACE(1, "Storing data:%d\n", data);
        TRACE(1, "Storing at addr:%d\n", addr);
        eeprom_write_byte(&romData[addr], data);

        addr += 1;
        eeprom_write_word(&romDataPnt, addr);

        return 1;
    }

    return 0;
}

#endif  // EEPROM_H__
