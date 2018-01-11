/***********************************************************************
 port from arduino project to avr-gcc
 https://github.com/Play-Zone/TM1638Module
*************************************************************************/
/***********************************************************************
 build:
 rm -f dest.*
 avr-gcc -mmcu=atmega168 -Wall -Wstrict-prototypes -Os -mcall-prologues -std=c99 -o dest.obj main.c;
 avr-objcopy -O ihex dest.obj dest.hex
 avrdude -C avrdude.conf -q -p atmega328p -c arduino -P /dev/ttyUSB0 -b 57600 -D -U flash:w:dest.hex:i
*************************************************************************/

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "delay.h"
#include "TM1638Lib.c"

void setup(void);
void loop(void);

//  Connections:
int DIO = 0; // PORTB0 ArduinoPin 8
int CLK = 1; // PORTB1 ArduinoPin 9
int STB = 2; // PORTB2 ArduinoPin 10

void setup()
{
  TM1638Lib(DIO, CLK, STB);
}
void loop()
{
  LedDisplay(1, 2, 3, 4, 5, 6, 7, 8);
  while (1)
  {
    unsigned int key_value = ReadKey();
    while(key_value)
    {
      LedDisplay(0, 0, 0, 0, 0, 0, key_value/10, key_value%10);
      break;
    }
  }
}

int main(void)
{
  setup();
  for (;;)
  {
    loop();
  }
  return 0;
}
