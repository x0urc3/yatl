# [Work-in-Progress] Yet Another Temperature Logger
A coin cell powered precision temperature logger

![PCB 3D view](/Hardware/output/3D_pcb.png)

# Development tools
1. [Kicad](https://www.kicad.org/)
2. [PlatformIO](https://platformio.org/)
3. Python/Bash

# Release
## Version 0.5
- Initial prototype
- ATMega48 microcontroller and LMT86 temperature sensor
- Temperature data is stored in the ATMega48 EEPROM. Sampled every 2 minutes for a duration of 8 hours
