#ifndef delay_h
#define delay_h

void delay_us(uint32_t us) // delay microseconds
{
  while (us)
  {
    _delay_us(1);
    us--;
  }
}

void delay_ms(uint32_t ms) // delay miliseconds
{
  while (ms)
  {
    _delay_ms(1);
    ms--;
  }
}

#endif // delay_h
