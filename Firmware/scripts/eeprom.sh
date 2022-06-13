#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

PIOPATH=$HOME'/.platformio/packages/'
AVRDUDEBIN=$PIOPATH'tool-avrdude/avrdude'
AVRDUDECONF=$PIOPATH'tool-avrdude/avrdude.conf'
#AVRDUDECMD= $AVRDUDEBIN' -C '$AVRDUDECONF' -c stk500v1 -p m328p -P /dev/ttyUSB0 -b 19200'

usage() {
    echo "Usage: $0 [OPTIONS]"
    echo "[OPTIONS]"
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
    FNAME='./data/'`date +%d%m%Y`'.hex'
    echo -e "${GREEN}Dump EEPROM data to $FNAME${NC}"
    $AVRDUDEBIN -C $AVRDUDECONF -c stk500v1 -p m328p -P /dev/ttyUSB0 -b 19200 -U eeprom:r:$FNAME:i
}

reset() {
    echo -e "${GREEN}Reset dirty flag${NC}"
    $AVRDUDEBIN -C $AVRDUDECONF -c stk500v1 -p m328p -P /dev/ttyUSB0 -b 19200 -t<<END
write eeprom 0x00 0xffff
quit
END
}

terminal() {
    echo -e "${GREEN}Avrdude terminal${NC}"
    $AVRDUDEBIN -C $AVRDUDECONF -c stk500v1 -p m328p -P /dev/ttyUSB0 -b 19200 -t
}

#Process arguments
while getopts "drt" arg; do
    case $arg in
        d)
            isp
            dump
            ;;
        r)
            isp
            reset
            ;;
        t)
            isp
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
