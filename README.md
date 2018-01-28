# AVR examples

``` cpp
compile:
avr-gcc -mmcu=atmega328 -Wall -Wstrict-prototypes -Os -mcall-prologues -std=c99 -o dest.obj main.c

create hex file:
avr-objcopy -O ihex dest.obj dest.hex

flash nano:
avrdude -C avrdude.conf -q -p atmega328p -c arduino -P /dev/ttyUSB0 -b 57600 -D -U flash:w:dest.hex:i

flash uno:
avrdude -C avrdude.conf -q -p atmega328p -c arduino -P /dev/ttyUSB0 -b 115200 -D -U flash:w:dest.hex:i
```
