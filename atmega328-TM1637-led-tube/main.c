/*

avr-gcc -mmcu=atmega168 -Wall -Wstrict-prototypes -Os -mcall-prologues -std=c99 -o dest.obj main.c; avr-objcopy -O ihex dest.obj dest.hex

avrdude -C avrdude.conf -q -p atmega328p -c arduino -P /dev/ttyUSB0 -b 57600 -D -U flash:w:dest.hex:i

*/

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
#include "1637.h"

int main(void)
{
  for(uint16_t i = 0; i < 9999; i++)
  {
    if(i>=9998)i=0;
    indication(i);
    delay_ms(250);
  }
}
