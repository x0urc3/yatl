#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

UC=m328p
UC=m48
PIOPATH=$HOME'/.platformio/packages/'
AVRDUDEBIN=$PIOPATH'tool-avrdude/avrdude'
AVRDUDECONF=$PIOPATH'tool-avrdude/avrdude.conf'
FNAME='./data/'`date +%d%m%Y`'.hex'

usage() {
    echo "Usage: $0 [OPTIONS]"
    echo "[OPTIONS]"
    echo -e "\t-i\tUpload ArduinoISP"
    echo -e "\t-d\tDump eeprom to file"
    echo -e "\t-r\tReset eeprom dirty flag"
    echo -e "\t-t\tAvrdude terminal"
    exit 0;
}

isp() {
    echo -e "${GREEN}Upload ArduinoISP${NC}"
    pio run -e isp -t upload
}

dump() {
    echo -e "${GREEN}Dump EEPROM data to $FNAME${NC}"
    $AVRDUDEBIN -C $AVRDUDECONF -c stk500v1 -p $UC -P /dev/ttyUSB0 -b 19200 -U eeprom:r:$FNAME:i
}

reset() {
    echo -e "${GREEN}Reset dirty flag${NC}"
    $AVRDUDEBIN -C $AVRDUDECONF -c stk500v1 -p $UC -P /dev/ttyUSB0 -b 19200 -t<<END
write eeprom 0x0 0xffff
quit
END
}

terminal() {
    echo -e "${GREEN}Avrdude terminal${NC}"
    $AVRDUDEBIN -C $AVRDUDECONF -c stk500v1 -p $UC -P /dev/ttyUSB0 -b 19200 -t
}

#Process arguments
while getopts "drti" arg; do
    case $arg in
        i)
            isp
            ;;
        d)
            FNAME='-'
            dump
            ;;
        r)
            reset
            ;;
        t)
            terminal
            ;;
        *)
            usage
            ;;
    esac
done

#Default to dump and reset
if (( $OPTIND == 1 )); then
    isp
    dump
    reset
fi
