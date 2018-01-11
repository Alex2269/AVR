/***********************************************************************
 port from arduino project to avr-gcc
 https://github.com/Play-Zone/TM1638Module
*************************************************************************/
/***********************************************************************
 build:
 
 rm -f dest.*
 avr-gcc -mmcu=atmega168 -Wall -Wstrict-prototypes -Os -mcall-prologues -std=c99 -o dest.obj generator-3.c;
 avr-objcopy -O ihex dest.obj dest.hex
 avrdude -C avrdude.conf -q -p atmega328p -c arduino -P /dev/ttyUSB0 -b 57600 -D -U flash:w:dest.hex:i
 
*************************************************************************/

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/delay_basic.h>
#include <stdint.h>
#include "delay.h"
#include "TM1638Lib.c"

#define LOW &=~
#define HIGH |=

//  Connections:
int DIO = 0; // PORTB0 ArduinoPin 8
int CLK = 1; // PORTB1 ArduinoPin 9
int STB = 2; // PORTB2 ArduinoPin 10

volatile uint32_t ms_count = 0;
volatile uint32_t ms_time = 0;
volatile uint32_t key_old = 0;
volatile uint32_t key_new = 0;
volatile uint32_t count = 0;

volatile uint8_t port_b_history = 0;
volatile uint8_t port_d_history = 0;
volatile uint8_t btn1 = 0;
volatile uint8_t btn2 = 0;

void setup(void);
void loop(void);
void impuls(uint32_t kol, uint32_t delay);

ISR (PCINT0_vect)
{
//   key_new++;
//   LedDisplay(0,0,0,0,0,key_new/10,key_new%10,0);
//   if(key_new>3)
//   {
//       count=0;
//   }
// 
//   PORTB LOW (1 << PORTB0); // turn On the Pull-up
//   PCICR LOW (1 << PCIE0); // set PCIE0 to enable PCMSK0 scan
//   PCMSK0 LOW (1 << PCINT0);
}

ISR (PCINT2_vect)
{
  uint8_t change_d_bits;
  change_d_bits = PIND ^ port_d_history;
  port_d_history = PIND;

  if(change_d_bits & (1 << PIND7)) // button PD7 -> ground
  {
    PORTD HIGH (1<<1)|(1<<0);
    loop();
  }
  //cli();
}

void initInterrupts(void)
{
  PORTB HIGH (1 << PORTB0); // turn On the Pull-up
  PCICR HIGH (1 << PCIE0); // set PCIE0 to enable PCMSK0 scan
  PCMSK0 HIGH (1 << PCINT0);

  DDRD LOW (1 << DDD7)|(1 << DDD6);
  PORTD HIGH (1 << PORTD7)|(1 << PORTD6);
  PCICR  HIGH (1 << PCIE2);
  PCMSK2 HIGH (1 << PCINT23)|(1 << PCINT22);
  sei();//enable interrupts
}

/***
 PCICR  HIGH (1 << PCIE0); // PCINT7-PCINT0
 PCICR  HIGH (1 << PCIE1); // PCINT14-PCINT8
 PCICR  HIGH (1 << PCIE2); // PCINT23-PCINT16

 PCMSK0 HIGH (1 << PCINT7)|(1 << PCINT6)|(1 << PCINT5)|(1 << PCINT4)|(1 << PCINT3)|(1 << PCINT2)|(1 << PCINT1)|(1 << PCINT0);
 PCMSK1 HIGH (1 << PCINT14)|(1 << PCINT13)|(1 << PCINT12)|(1 << PCINT11)|(1 << PCINT10)|(1 << PCINT9)|(1 << PCINT8);
 PCMSK2 HIGH (1 << PCINT23)|(1 << PCINT22)|(1 << PCINT21)|(1 << PCINT20)|(1 << PCINT19)|(1 << PCINT18)|(1 << PCINT17)|(1 << PCINT16);
***/

void setup()
{
  TM1638Lib(DIO, CLK, STB);
}

void loop()
{
  LedDisplay(1, 2, 3, 4, 5, 6, 7, 8);
  while (1)
  {
    key_old = ReadKey();
    while(key_old)
    {
      if(key_old==16)
      {
        LedDisplay(8,8,8,8,8,8,8,8);
        break;
      }
      else
      {
        LedDisplay(0, 0, 0, 0, 0, 0, key_old/10, key_old%10);
        impuls(key_old, key_old); //(ms_count, ms_time)
      }
      break;
    }
  }
}

void impuls(uint32_t kol, uint32_t delay)
{
  initInterrupts();
  count = kol*10;

  DDRD HIGH (1<<1)|(1<<0);
  PORTD LOW (1<<1)|(1<<0);
  while (count)
  {
    if(count<2)
    {
      PORTD HIGH (1<<1)|(1<<0);
      return;
    }
    PORTD HIGH (1<<1);
    PORTD LOW (1<<0);
    //_delay_loop_2(delay*60000);
    delay_us(delay*12000);
    PORTD HIGH (1<<0);
    PORTD LOW (1<<1);
    //_delay_loop_2(delay*60000);
    delay_us(delay*12000);
    count--;
  }
  PORTD HIGH (1<<1)|(1<<0);
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
