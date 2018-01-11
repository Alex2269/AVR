/***********************************************************************
 port from arduino project to avr-gcc
 https://github.com/Play-Zone/TM1638Module
*************************************************************************/

#include "TM1638Lib.h"
#define _LOW &=~
#define _HIGH |=

unsigned char tab[32] =
{
  0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,
  0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
  0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,
  0xFF,0xEF,0xF7,0xFC,0xB9,0xDE,0xF9,0xF1
};

void TM1638Lib(int DIO, int CLK, int STB)
{
  DDRB=0;
  DDRB _HIGH (1<<STB);
  DDRB _HIGH (1<<CLK);
  DDRB _HIGH (1<<DIO);
  PORTB _LOW (1<<STB);
  PORTB _LOW (1<<CLK);
  PORTB _LOW (1<<DIO);
  unsigned char i;
  Write_COM(0x8b);
  Write_COM(0x40);
  PORTB _LOW (1<<STB);
  TM1638_Write(0xc0);
  for (i = 0; i<16; i++)
    TM1638_Write(0x00);
  PORTB _HIGH (1<<STB);
}

void LedDisplay(unsigned char data0, unsigned char data1, unsigned char data2, unsigned char data3, unsigned char data4, unsigned char data5, unsigned char data6, unsigned char data7)
{
  unsigned char writedata0, writedata1, writedata2, writedata3, writedata4, writedata5, writedata6, writedata7;
  writedata0 = 
  (tab[data7] & 0x01) +
  ((tab[data6] & 0x01) << 1) +
  ((tab[data5] & 0x01) << 2) +
  ((tab[data4] & 0x01) << 3) +
  ((tab[data3] & 0x01) << 4) +
  ((tab[data2] & 0x01) << 5) +
  ((tab[data1] & 0x01) << 6) +
  ((tab[data0] & 0x01) << 7);

  writedata1 =
  ((tab[data7] & 0x02) >> 1) +
  ((tab[data6] & 0x02)) +
  ((tab[data5] & 0x02) << 1) +
  ((tab[data4] & 0x02) << 2) +
  ((tab[data3] & 0x02) << 3) +
  ((tab[data2] & 0x02) << 4) +
  ((tab[data1] & 0x02) << 5) +
  ((tab[data0] & 0x02) << 6);

  writedata2 =
  ((tab[data7] & 0x04) >> 2) +
  ((tab[data6] & 0x04) >> 1) +
  ((tab[data5] & 0x04)) +
  ((tab[data4] & 0x04) << 1) +
  ((tab[data3] & 0x04) << 2) +
  ((tab[data2] & 0x04) << 3) +
  ((tab[data1] & 0x04) << 4) +
  ((tab[data0] & 0x04) << 5);

  writedata3 =
  ((tab[data7] & 0x08) >> 3) +
  ((tab[data6] & 0x08) >> 2) +
  ((tab[data5] & 0x08) >> 1) +
  ((tab[data4] & 0x08)) +
  ((tab[data3] & 0x08) << 1) +
  ((tab[data2] & 0x08) << 2) +
  ((tab[data1] & 0x08) << 3) +
  ((tab[data0] & 0x08) << 4);

  writedata4 =
  ((tab[data7] & 0x10) >> 4) +
  ((tab[data6] & 0x10) >> 3) +
  ((tab[data5] & 0x10) >> 2) +
  ((tab[data4] & 0x10) >> 1) +
  ((tab[data3] & 0x10)) +
  ((tab[data2] & 0x10) << 1) +
  ((tab[data1] & 0x10) << 2) +
  ((tab[data0] & 0x10) << 3);

  writedata5 =
  ((tab[data7] & 0x20) >> 5) +
  ((tab[data6] & 0x20) >> 4) +
  ((tab[data5] & 0x20) >> 3) +
  ((tab[data4] & 0x20) >> 2) +
  ((tab[data3] & 0x20) >> 1) +
  ((tab[data2] & 0x20)) +
  ((tab[data1] & 0x20) << 1) +
  ((tab[data0] & 0x20) << 2);

  writedata6 =
  ((tab[data7] & 0x40) >> 6) +
  ((tab[data6] & 0x40) >> 5) +
  ((tab[data5] & 0x40) >> 4) +
  ((tab[data4] & 0x40) >> 3) +
  ((tab[data3] & 0x40) >> 2) +
  ((tab[data2] & 0x40) >> 1) +
  ((tab[data1] & 0x40)) +
  ((tab[data0] & 0x40) << 1);

  writedata7 =
  ((tab[data7] & 0x80) >> 7) +
  ((tab[data6] & 0x80) >> 6) +
  ((tab[data5] & 0x80) >> 5) +
  ((tab[data4] & 0x80) >> 4) +
  ((tab[data3] & 0x80) >> 3) +
  ((tab[data2] & 0x80) >> 2) +
  ((tab[data1] & 0x80) >> 1) +
  ((tab[data0] & 0x80));

  Write_COM(0x8a);
  Write_COM(0x40);
  PORTB _LOW (1<<STB);
  TM1638_Write(0xc0);
  TM1638_Write(writedata0);
  TM1638_Write(0x80);
  TM1638_Write(writedata1);
  TM1638_Write(0x00);
  TM1638_Write(writedata2);
  TM1638_Write(0x00);
  TM1638_Write(writedata3);
  TM1638_Write(0x00);
  TM1638_Write(writedata4);
  TM1638_Write(0x00);
  TM1638_Write(writedata5);
  TM1638_Write(0x00);
  TM1638_Write(writedata6);
  TM1638_Write(0x00);
  TM1638_Write(writedata7);
  TM1638_Write(0x00);
  PORTB _HIGH (1<<STB);
}

void TM1638_Write(unsigned char DATA)
{
  unsigned char i;
  PORTB _LOW (1<<DIO);
  for (i = 0; i<8; i++)
  {
    PORTB _LOW (1<<CLK);
    if (DATA & 0X01)
      PORTB _HIGH (1<<DIO);
    else
      PORTB _LOW (1<<DIO);
    DATA >>= 1;
    PORTB _HIGH (1<<CLK);
  }
}

unsigned char TM1638_Read(void)
{
  unsigned char i;
  unsigned char temp = 0;
  PORTB _HIGH (1<<DIO);
  for (i = 0; i<8; i++)
  {
    temp >>= 1;
    PORTB _LOW (1<<CLK);
    if ((bit_is_clear(PINB, 0)) ==0)
      temp _HIGH 0x40;
    PORTB _HIGH (1<<CLK);
  }
  return temp;
}

void Write_COM(unsigned char cmd)
{
  PORTB _LOW (1<<STB);
  TM1638_Write(cmd);
  PORTB _HIGH (1<<STB);
}

unsigned char ReadKey(void)
{
  unsigned char c[4], i, key_value = 0;
  PORTB _LOW (1<<STB);
  TM1638_Write(0x42);
  delay_ms(100);
  for (i = 0; i<4; i++)
  {
    c[i] = TM1638_Read();
  }
  PORTB _HIGH (1<<STB);
  if (c[0] == 0x04) key_value = 1;
  if (c[0] == 0x40) key_value = 2;
  if (c[1] == 0x04) key_value = 3;
  if (c[1] == 0x40) key_value = 4;
  if (c[2] == 0x04) key_value = 5;
  if (c[2] == 0x40) key_value = 6;
  if (c[3] == 0x04) key_value = 7;
  if (c[3] == 0x40) key_value = 8;
  if (c[0] == 0x02) key_value = 9;
  if (c[0] == 0x20) key_value = 10;
  if (c[1] == 0x02) key_value = 11;
  if (c[1] == 0x20) key_value = 12;
  if (c[2] == 0x02) key_value = 13;
  if (c[2] == 0x20) key_value = 14;
  if (c[3] == 0x02) key_value = 15;
  if (c[3] == 0x20) key_value = 16;
  return (key_value);
}

void Write_DATA(unsigned char add, unsigned char DATA)
{
  Write_COM(0x44);
  PORTB _LOW (1<<STB);
  TM1638_Write(0xc0 | add);
  TM1638_Write(DATA);
  PORTB _HIGH (1<<STB);
}
