#define F_CPU 8000000 //fuse LOW=0x7a
#include <avr/io.h>
#include <stdint.h> // needed for uint16_t
#include <avr/interrupt.h>
#include <util/delay.h>

#define nop __asm__ volatile("nop")
//       for(uint16_t i=30;i>0;i--) nop ;

uint16_t step = 1;
float PI = 3.14;
float tmp = 0;
int delay = 1;

void sine_gen(void);

void delay_us(uint16_t us) //функция задержки
{
  while (us)
  {
    _delay_us(1);
    us--;
  }
}

#define sinus_points 128
uint16_t sin_table_a[sinus_points];
uint16_t sin_table_b[sinus_points];
uint16_t sin_table_c[sinus_points];
uint16_t sin_table_d[sinus_points];
uint16_t sin_table_e[sinus_points];

void sin_init(uint16_t i)
{
  sin_table_a[(i+(sinus_points/5*1)) & ((sinus_points-1))]=
  sin_table_b[(i+(sinus_points/5*2)) & ((sinus_points-1))]=
  sin_table_c[(i+(sinus_points/5*3)) & ((sinus_points-1))]=
  sin_table_d[(i+(sinus_points/5*4)) & ((sinus_points-1))]=
  sin_table_e[(i+(sinus_points/5*5)) & ((sinus_points-1))]=
  (uint16_t)((sin ((float) i*(2*PI/sinus_points))+1)*(sinus_points/2-1));
}

void pwm(uint16_t a, uint16_t b, uint16_t c, uint16_t d, uint16_t e)
{
   uint16_t i;
   DDRB=0b00111111; // DDRx: 0 -pins to input
   DDRD=0b11111100; // DDRx: 1 -pins to output
   for(i= 0; i < sinus_points; i+=step)
   {

     if(i<(a & (sinus_points-1)))
     {
       PORTD  &=~ (1<<PD2); // 0
       for(uint16_t i=30;i>0;i--) nop ;
       PORTD   |= (1<<PD3); // 1
     }
     else
     {
       PORTD  &=~ (1<<PD3); // 0
       for(uint16_t i=30;i>0;i--) nop ;
       PORTD   |= (1<<PD2); // 1
     }

     if(i<(b & (sinus_points-1)))
     {
       PORTD  &=~ (1<<PD4); // 0
       for(uint16_t i=30;i>0;i--) nop ;
       PORTD   |= (1<<PD5); // 1
     }
     else
     {
       PORTD  &=~ (1<<PD5); // 0
       for(uint16_t i=30;i>0;i--) nop ;
       PORTD   |= (1<<PD4); // 1
     }

     if(i<(c & (sinus_points-1)))
     {
       PORTD  &=~ (1<<PD6); // 0
       PORTB  &=~ (1<<PB4); // 0
       for(uint16_t i=30;i>0;i--) nop ;
       PORTD   |= (1<<PD7); // 1
       PORTB   |= (1<<PB5); // 1
     }
     else
     {
       PORTD  &=~ (1<<PD7); // 0
       PORTB  &=~ (1<<PB5); // 0
       for(uint16_t i=30;i>0;i--) nop ;
       PORTD   |= (1<<PD6); // 1
       PORTB   |= (1<<PB4); // 1
     }

     if(i<(d & (sinus_points-1)))
     {
       PORTB  &=~ (1<<PB0); // 0
       for(uint16_t i=30;i>0;i--) nop ;
       PORTB   |= (1<<PB1); // 1
     }
     else
     {
       PORTB  &=~ (1<<PB1); // 0
       for(uint16_t i=30;i>0;i--) nop ;
       PORTB   |= (1<<PB0); // 1
     }

     if(i<(e & (sinus_points-1)))
     {
       PORTB  &=~ (1<<PB2); // 0
       for(uint16_t i=30;i>0;i--) nop ;
       PORTB   |= (1<<PB3); // 1
     }
     else
     {
       PORTB  &=~ (1<<PB3); // 0
       for(uint16_t i=30;i>0;i--) nop ;
       PORTB   |= (1<<PB2); // 1
     }

     if(delay)
     delay_us(delay);
   }
}

void sine_gen(void)
{
  uint16_t i,a,b,c,d,e;

  tmp += 0.5;
  if (tmp>=1) { tmp = 0; delay --; }
  if (delay<0) delay = 0;

  for (i=0; i<sinus_points; i+=step)
  {
    a=(sin_table_a[i & (sinus_points-1)]);
    b=(sin_table_b[i & (sinus_points-1)]);
    c=(sin_table_c[i & (sinus_points-1)]);
    d=(sin_table_d[i & (sinus_points-1)]);
    e=(sin_table_e[i & (sinus_points-1)]);
    pwm(a,b,c,d,e);
  }
  return;
}

int main (void)
{
  uint16_t i;    
  for (i=0; i<sinus_points; i++)
  {
    sin_init(i);
  }

  while(1){
   sine_gen();
  }
  return 0;
}

/*
avr-gcc -mmcu=atmega328 -Wall -g -Os -Werror -lm -mcall-prologues -std=c99 -o avr-pwm.obj avr-pwm-blink.c; avr-objcopy -O ihex avr-pwm.obj avr-pwm.hex

avrdude -C avrdude.conf -q -p atmega328p -c arduino -P /dev/ttyUSB0 -b 57600 -D -U flash:w:avr-pwm.hex:i

avrdude -c usbasp -p m328p -v -e -U flash:w:avr-pwm.hex:i

*/
