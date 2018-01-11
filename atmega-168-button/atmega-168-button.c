/*
avr-gcc -mmcu=atmega168 -Wall -Wstrict-prototypes -Os -mcall-prologues -std=c99 -o dest.obj atmega-168-button.c; avr-objcopy -O ihex dest.obj dest.hex
avrdude -C avrdude.conf -q -p atmega328p -c arduino -P /dev/ttyUSB0 -b 57600 -D -U flash:w:dest.hex:i
*/

#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#define LOW &=~
#define HIGH |=

volatile uint8_t port_b_history = 0;
volatile uint8_t port_d_history = 0;
volatile uint8_t btn1 = 0;
volatile uint8_t btn2 = 0;

void delay_us(uint8_t us) // delay microseconds
{
  while (us)
  {
    _delay_us(1);
    us--;
  }
}

void delay_ms(uint8_t ms) // delay miliseconds
{
  while (ms)
  {
    _delay_ms(1);
    ms--;
  }
}

ISR (PCINT0_vect)
{
  uint8_t change_b_bits;
  change_b_bits = PINB ^ port_b_history;
  port_b_history = PINB;

  if(change_b_bits & (1 << PINB0)) // button PB0 -> ground
  {
    DDRD|=(1<<3);
    PORTD HIGH (1<<3);
  }
  if(change_b_bits & (1 << PINB1)) // button PB1 -> ground
  {
    DDRD|=(1<<3);
    PORTD LOW (1<<3);
  }
  if(change_b_bits & (1 << PINB2))
  {
    DDRD|=(1<<4);
    PORTD HIGH (1<<4);
  }
  if(change_b_bits & (1 << PINB3))
  {
    DDRD|=(1<<4);
    PORTD LOW (1<<4);
  }
  if(change_b_bits & (1 << PINB4))
  {
    DDRD|=(1<<5);
    PORTD HIGH (1<<5);
  }
  if(change_b_bits & (1 << PINB5))
  {
    DDRD|=(1<<5);
    PORTD LOW (1<<5);
  }
}

ISR (PCINT2_vect)
{
  uint8_t change_d_bits;
  change_d_bits = PIND ^ port_d_history;
  port_d_history = PIND;

  if(change_d_bits & (1 << PIND6)) // button PD6 -> ground
  {
    btn1++;
    btn2=0;
    DDRD|=(1<<5)|(1<<4)|(1<<3);
    PORTD HIGH (1<<3);
    PORTD HIGH (1<<4);
    PORTD HIGH (1<<5);
  }

  if(change_d_bits & (1 << PIND7)) // button PD7 -> ground
  {
    btn2++;
    btn1=0;
    DDRD|=(1<<5)|(1<<4)|(1<<3);

    if(btn2>=1){
    PORTD LOW (1<<3);
    }
    if(btn2>=3){
    PORTD LOW (1<<4);
    }
    if(btn2>=5){
    PORTD LOW (1<<5);
    }
    if(btn2>=7){
    DDRD|=(1<<5)|(1<<4)|(1<<3);
    PORTD = PORTD ^= 0b00111000;
    }
    if(btn2>=8){
    btn2=0;
    }
  }
}

void initInterrupt0(void)
{
  DDRB &= ~(1 << DDB5)|(1 << DDB4)|(1 << DDB3)|(1 << DDB2)|(1 << DDB1)|(1 << DDB0); // Clear the PortB pin0 & pin1
  PORTB |= (1 << PORTB5)|(1 << PORTB4)|(1 << PORTB3)|(1 << PORTB2)|(1 << PORTB1)|(1 << PORTB0); // turn On the Pull-up
  PCICR |= (1 << PCIE0); // set PCIE0 to enable PCMSK0 scan
  PCMSK0 |= (1 << PCINT5)|(1 << PCINT4)|(1 << PCINT3)|(1 << PCINT2)|(1 << PCINT1)|(1 << PCINT0);

  DDRD &= ~(1 << DDD7)|(1 << DDD6);
  PORTD |= (1 << PORTD7)|(1 << PORTD6);
  PCICR  |= (1 << PCIE2);
  PCMSK2 |= (1 << PCINT23)|(1 << PCINT22); 

  /* PCICR  |= (1 << PCIE0); // PCINT7-PCINT0
     PCMSK0 |= (1 << PCINT7)|(1 << PCINT6)|(1 << PCINT5)|(1 << PCINT4)|(1 << PCINT3)|(1 << PCINT2)|(1 << PCINT1)|(1 << PCINT0);
     PCICR  |= (1 << PCIE1); // PCINT14-PCINT8
     PCMSK1 |= (1 << PCINT14)|(1 << PCINT13)|(1 << PCINT12)|(1 << PCINT11)|(1 << PCINT10)|(1 << PCINT9)|(1 << PCINT8);
     PCICR  |= (1 << PCIE2); // PCINT23-PCINT16
     PCMSK2 |= (1 << PCINT23)|(1 << PCINT22)|(1 << PCINT21)|(1 << PCINT20)|(1 << PCINT19)|(1 << PCINT18)|(1 << PCINT17)|(1 << PCINT16); */
}

int main( void )
{
  initInterrupt0();
  sei();
  while(1)
  {
  };
}
