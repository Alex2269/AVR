
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
#include "delay.h"
#include "max7219display.c"

// 8 digit display connected to these pins

#define pinCS  PB2
#define pinCLK PB3
#define pinDIN PB4

long num;
void test_display(void);
void scroll_digits(void);

// showNumberDec(num, leading_zero, length, pos);

void test_display(void)
{
  showNumberDec(12345678, 1, 8, 0);
  delay_ms(500);
  showNumberDec(87654321, 1, 8, 0);
  delay_ms(500);
  showNumberDec(88888888, 1, 8, 0);
  delay_ms(500);
  num = 0L;
}

void scroll_digits(void)
{
  showNumberDec(num, 0, 8, 0);
  delay_ms(1);
  if (++num > 99999999)
    num = 0L;
}

int main(void)
{
  MAX7219_init(pinCLK, pinDIN, pinCS);
  test_display();
  while(1)
  {
     scroll_digits();
  }
}
