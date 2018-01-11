#ifndef _DELAY_H
#define _DELAY_H

void delay_us(uint16_t us);
void delay_ms(uint16_t ms);

void delay_us(uint16_t us)
{
  while (us)
  {
    _delay_us(1);
    us--;
  }
}

void delay_ms(uint16_t ms)
{
  while (ms)
  {
    _delay_ms(1);
    ms--;
  }
}

#endif /* _DELAY_H */
