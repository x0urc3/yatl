#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

PIOPATH=$HOME'/.platformio/packages/'
AVRDUDEBIN=$PIOPATH'tool-avrdude/avrdude'
AVRDUDECONF=$PIOPATH'tool-avrdude/avrdude.conf'
AVRDUDECMD= $AVRDUDEBIN' -C '$AVRDUDECONF' -c stk500v1 -p m328p -P /dev/ttyUSB0 -b 19200'

FNAME='./data/'`date +%d%m%Y`'.hex'
echo -e "${GREEN}Dump EEPROM data to $FNAME${NC}"
#pio run -e isp -t upload
$AVRDUDEBIN -C $AVRDUDECONF -c stk500v1 -p m328p -P /dev/ttyUSB0 -b 19200 -U eeprom:r:$FNAME:i

echo -e "${GREEN}Remove dirty flag${NC}"
$AVRDUDEBIN -C $AVRDUDECONF -c stk500v1 -p m328p -P /dev/ttyUSB0 -b 19200 -t<<END
write eeprom 0x00 0xff
quit
END
