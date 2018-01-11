
#define SET_PORT  DDRD
#define SEND_PORT PORTD

#define _clk_a PORTD2
#define _dio_a PORTD3

#define _clk_b PORTD4
#define _dio_b PORTD5

#define _LOW &=~
#define _HIGH |=

void delay_us(uint16_t us);
void delay_ms(uint16_t ms);

void indication_a(int n);
void indication_b(int n);
void write_a(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth);
void write_b(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth);
void start_a(void);
void start_b(void);
void stop_a(void);
void stop_b(void);
void writeValue_a(uint8_t value);
void writeValue_b(uint8_t value);

uint8_t digits[] =
{
  0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f
};

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

void indication_a(int n)
{
  int n1, n2, n3, n4;
  n1 = (int) n % 10;
  n2 = (int) ((n % 100)-n1)/10;
  n3 = (int) ((n % 1000) - n2 - n1) / 100;
  n4 = (int) ((n % 10000) - n3 - n2 - n1) / 1000;
  write_a(digits[n4], digits[n3], digits[n2], digits[n1]);
}

void indication_b(int n)
{
  int n1, n2, n3, n4;
  n1 = (int) n % 10;
  n2 = (int) ((n % 100)-n1)/10;
  n3 = (int) ((n % 1000) - n2 - n1) / 100;
  n4 = (int) ((n % 10000) - n3 - n2 - n1) / 1000;
  write_b(digits[n4], digits[n3], digits[n2], digits[n1]);
}

void write_a(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth)
{
  start_a();
  writeValue_a(0x8c);
  start_a();
  writeValue_a(0x40);
  stop_a();
  start_a();
  writeValue_a(0xc0);
  writeValue_a(first);
  writeValue_a(second);
  writeValue_a(third);
  writeValue_a(fourth);
  stop_a();
}

void write_b(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth)
{
  start_b();
  writeValue_b(0x8c);
  start_b();
  writeValue_b(0x40);
  stop_b();
  start_b();
  writeValue_b(0xc0);
  writeValue_b(first);
  writeValue_b(second);
  writeValue_b(third);
  writeValue_b(fourth);
  stop_b();
}

void start_a(void)
{
  SET_PORT|=(1<<_clk_a);
  SET_PORT|=(1<<_dio_a);
  SEND_PORT _HIGH (1<<_clk_a);
  SEND_PORT _HIGH (1<<_dio_a);
  delay_us(5);
  SEND_PORT _LOW (1<<_dio_a);
  SEND_PORT _LOW (1<<_clk_a);
  delay_us(5);
}

void start_b(void)
{
  SET_PORT|=(1<<_clk_b);
  SET_PORT|=(1<<_dio_b);
  SEND_PORT _HIGH (1<<_clk_b);
  SEND_PORT _HIGH (1<<_dio_b);
  delay_us(5);
  SEND_PORT _LOW (1<<_dio_b);
  SEND_PORT _LOW (1<<_clk_b);
  delay_us(5);
}

void stop_a(void)
{
  SET_PORT|=(1<<_clk_a);
  SET_PORT|=(1<<_dio_a);
  SEND_PORT _LOW (1<<_clk_a);
  SEND_PORT _LOW (1<<_dio_a);
  delay_us(5);
  SEND_PORT _HIGH (1<<_clk_a);
  SEND_PORT _HIGH (1<<_dio_a);
  delay_us(5);
}

void stop_b(void)
{
  SET_PORT|=(1<<_clk_b);
  SET_PORT|=(1<<_dio_b);
  SEND_PORT _LOW (1<<_clk_b);
  SEND_PORT _LOW (1<<_dio_b);
  delay_us(5);
  SEND_PORT _HIGH (1<<_clk_b);
  SEND_PORT _HIGH (1<<_dio_b);
  delay_us(5);
}

void writeValue_a(uint8_t value)
{
  for(uint8_t i = 0; i < 8; i++)
  {
    SEND_PORT _LOW (1<<_clk_a);
    delay_us(5);
    bool not_null = ((value & (1 << i)) >> i);
    if(not_null)
      SEND_PORT _HIGH (1<<_dio_a);
    else
      SEND_PORT _LOW (1<<_dio_a);
    delay_us(5);
    SEND_PORT _HIGH (1<<_clk_a);
    delay_us(5);
  }
  SEND_PORT _LOW (1<<_clk_a);
  delay_us(5);
  SEND_PORT _HIGH (1<<_clk_a);
  delay_us(5);
}

void writeValue_b(uint8_t value)
{
  for(uint8_t i = 0; i < 8; i++)
  {
    SEND_PORT _LOW (1<<_clk_b);
    delay_us(5);
    bool not_null = ((value & (1 << i)) >> i);
    if(not_null)
      SEND_PORT _HIGH (1<<_dio_b);
    else
      SEND_PORT _LOW (1<<_dio_b);
    delay_us(5);
    SEND_PORT _HIGH (1<<_clk_b);
    delay_us(5);
  }
  SEND_PORT _LOW (1<<_clk_b);
  delay_us(5);
  SEND_PORT _HIGH (1<<_clk_b);
  delay_us(5);
}
