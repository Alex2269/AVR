/*

avr-gcc -mmcu=atmega168 -Wall -Wstrict-prototypes -Os -mcall-prologues -std=c99 -o dest.obj atmega-328-generator.c; avr-objcopy -O ihex dest.obj dest.hex

avrdude -C avrdude.conf -q -p atmega328p -c arduino -P /dev/ttyUSB0 -b 57600 -D -U flash:w:dest.hex:i

*/

#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
#include "1637_PORTD.h"

#define LOW &=~
#define HIGH |=

int pressed0=0;
int pressed1=0;
int pressed2=0;
int pressed3=0;
int pressed0_x=0;
int pressed1_x=0;
int pressed2_x=0;
int pressed3_x=0;

int pressed4=0;
int pressed5=0;

volatile uint16_t indicate_a = 0;
volatile uint16_t indicate_b = 0;
volatile uint16_t ms_count = 0;
volatile uint16_t ms_time = 0;

void forceHardReset(void);
void wdt_init(void);
void impuls_ms_count_time(uint16_t kol, uint16_t delay);

void initInterrupt0(void)
{
  DDRB LOW (1 << DDB5)|(1 << DDB4)|(1 << DDB3)|(1 << DDB2)|(1 << DDB1)|(1 << DDB0);
  PORTB HIGH (1 << PORTB5)|(1 << PORTB4)|(1 << PORTB3)|(1 << PORTB2)|(1 << PORTB1)|(1 << PORTB0);
}

void impuls_ms_count_time(uint16_t kol, uint16_t delay)
{
  uint16_t count = kol*10;
  DDRD HIGH (1<<7)|(1<<6);
  PORTD LOW (1<<7)|(1<<6);
  while (count)
  {
//     if (bit_is_clear(PINB, 5)) pressed5++;
//     if (bit_is_set(PINB, 5)) pressed5=0;
//     if (pressed5>100)
//      {
//       pressed5=0;
//       indicate_a = count = ms_count = 0;
//       indicate_b = count = ms_time = 0;
//       return;
//      }

    indication_a(count/10);
    PORTD HIGH (1<<2);
    PORTD LOW (1<<1);
    delay_us(delay*100000);
    indication_b(count);
    PORTD HIGH (1<<1);
    PORTD LOW (1<<2);
    delay_us(delay*100000);
    count--;
  }
}

int main( void )
{
  initInterrupt0();
  sei();
  while(1)
  {
    indication_a(indicate_a);
    indication_b(indicate_b);
    if(ms_time<1)ms_time=0;
    if(ms_time>1000)ms_time=1000;
    if(ms_count<1)ms_count=0;
    if(ms_count>1000)ms_count=1000;
    
    if (bit_is_clear(PINB, 0)) pressed0++;
    if (pressed0>50) 
    {
      pressed0=0;
      pressed0_x++;
      indicate_a = ms_count+=1;
      if (pressed0_x>25) 
      {
        indicate_a = ms_count+=24;
        delay_ms(350);
      }
    }
    if (bit_is_set(PINB, 0))
    {
      pressed0=0;
      pressed0_x=0;
    }

    if (bit_is_clear(PINB, 1)) pressed1++;
    if (pressed1>50) 
    {
      pressed1=0;
      pressed1_x++;
      indicate_a = ms_count-=1;
      if (pressed1_x>25) 
      {
        indicate_a = ms_count-=24;
        delay_ms(350);
      }
    }
    if (bit_is_set(PINB, 1))
    {
      pressed1=0;
      pressed1_x=0;
    }

    if (bit_is_clear(PINB, 2)) pressed2++;
    if (pressed2>50) 
    {
      pressed2=0;
      pressed2_x++;
      indicate_b = ms_time+=1;
      if (pressed2_x>25) 
      {
        indicate_b = ms_time+=24;
        delay_ms(350);
      }
    }
    if (bit_is_set(PINB, 2))
    {
      pressed2=0;
      pressed2_x=0;
    }

    if (bit_is_clear(PINB, 3)) pressed3++;
    if (pressed3>50) 
    {
      pressed3=0;
      pressed3_x++;
      indicate_b = ms_time-=1;
      if (pressed3_x>25) 
      {
        indicate_b = ms_time-=24;
        delay_ms(350);
      }
    }
    if (bit_is_set(PINB, 3))
    {
      pressed3=0;
      pressed3_x=0;
    }

    if (bit_is_clear(PINB, 4)) pressed4++;
    if (pressed4>50)
    {
      pressed4=0;
      impuls_ms_count_time(ms_count, ms_time);
    }
  };
}
