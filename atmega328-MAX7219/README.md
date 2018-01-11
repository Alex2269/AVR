# max7219display
LED-7-Segment-module-8-digits, avr-gcc 

compile:

avr-gcc -mmcu=atmega168 -Wall -Wstrict-prototypes -Os -mcall-prologues -std=c99 -o dest.obj main_Display.c

avr-objcopy -O ihex dest.obj dest.hex

flash:

avrdude -C avrdude.conf -q -p atmega328p -c arduino -P /dev/ttyUSB0 -b 57600 -D -U flash:w:dest.hex:i
