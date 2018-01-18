#include <util/delay.h>

void delay_us(uint32_t us) //функция задержки
{
  while (us)
  {
    _delay_us(1);
    us--;
  }
}

void delay_ms(uint32_t ms) //функция задержки
{
  while (ms)
  {
    _delay_ms(1);
    ms--;
  }
}

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(13, LOW);  
}

// the loop function runs over and over again forever
void loop() {
    float j;
    for(j=2500;j>1000;j-=2)
    {
     digitalWrite(2, HIGH);
     digitalWrite(3, LOW);
     digitalWrite(4, HIGH);
     digitalWrite(5, LOW);
     digitalWrite(6, HIGH);
     digitalWrite(7, LOW);
     digitalWrite(8, HIGH);
     digitalWrite(9, LOW);
     delay_us(j);
     digitalWrite(2, LOW);
     digitalWrite(3, HIGH);
     digitalWrite(4, LOW);
     digitalWrite(5, HIGH);
     digitalWrite(6, LOW);
     digitalWrite(7, HIGH);
     digitalWrite(8, LOW);
     digitalWrite(9, HIGH);
     delay_us(j);
    }
    for(j=1000;j<2500;j+=2)
    {
     digitalWrite(2, HIGH);
     digitalWrite(3, LOW);
     digitalWrite(4, HIGH);
     digitalWrite(5, LOW);
     digitalWrite(6, HIGH);
     digitalWrite(7, LOW);
     digitalWrite(8, HIGH);
     digitalWrite(9, LOW);
     delay_us(j);
     digitalWrite(2, LOW);
     digitalWrite(3, HIGH);
     digitalWrite(4, LOW);
     digitalWrite(5, HIGH);
     digitalWrite(6, LOW);
     digitalWrite(7, HIGH);
     digitalWrite(8, LOW);
     digitalWrite(9, HIGH);
     delay_us(j);
    }
    digitalWrite(13, HIGH);
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    delay_ms(4000);
    digitalWrite(13, LOW);
///!
    for(j=1500;j>1000;j-=1)
    {
     digitalWrite(2, HIGH);
     digitalWrite(3, LOW);
     digitalWrite(4, HIGH);
     digitalWrite(5, LOW);
     digitalWrite(6, HIGH);
     digitalWrite(7, LOW);
     digitalWrite(8, HIGH);
     digitalWrite(9, LOW);
     delay_us(j);
     digitalWrite(2, LOW);
     digitalWrite(3, HIGH);
     digitalWrite(4, LOW);
     digitalWrite(5, HIGH);
     digitalWrite(6, LOW);
     digitalWrite(7, HIGH);
     digitalWrite(8, LOW);
     digitalWrite(9, HIGH);
     delay_us(j);
    }
    for(j=1000;j<1500;j+=1)
    {
     digitalWrite(2, HIGH);
     digitalWrite(3, LOW);
     digitalWrite(4, HIGH);
     digitalWrite(5, LOW);
     digitalWrite(6, HIGH);
     digitalWrite(7, LOW);
     digitalWrite(8, HIGH);
     digitalWrite(9, LOW);
     delay_us(j);
     digitalWrite(2, LOW);
     digitalWrite(3, HIGH);
     digitalWrite(4, LOW);
     digitalWrite(5, HIGH);
     digitalWrite(6, LOW);
     digitalWrite(7, HIGH);
     digitalWrite(8, LOW);
     digitalWrite(9, HIGH);
     delay_us(j);
    }
    digitalWrite(13, HIGH);
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    delay_ms(4000);
    digitalWrite(13, LOW);
}


