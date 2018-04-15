#include "74HC595.h"

#define _DIO PD5
#define _RCLK PD6
#define _SCLK PD7

unsigned char _LED_TAB[29]=
{
  0xC0, //0
  0xF9, //1
  0xA4, //2
  0xB0, //3
  0x99, //4
  0x92, //5
  0x82, //6
  0xF8, //7
  0x80, //8
  0x90, //9
  0x88, //A
  0x83, //b
  0xC6, //C
  0xA1, //d
  0x86, //E
  0x8E, //F
  0xC2, //G
  0x89, //H
  0xF9, //I
  0xF1, //J
  0xC3, //L
  0xA9, //n
  0xC0, //O
  0x8C, //P
  0x98, //q
  0x92, //S
  0xC1, //U
  0x91, //Y
  0xFE //hight -
};

void send(unsigned char X)
{
  for (int i = 8; i >= 1; i--)
  {
    if (X & 0x80)
    {
      PORTD HIGH (1<<_DIO);
    }
    else
    {
      PORTD LOW (1<<_DIO);
    }
    X <<= 1;
    PORTD LOW (1<<_SCLK);
    PORTD HIGH (1<<_SCLK);
  }
}

void pre_send(unsigned char X, unsigned char value)
{
  DDRD = 0xFF; // Выходы на сегменты
  PORTD = 0b11100000; // Ноль на выходе
  send(X);
  send(value);
  PORTD LOW (1<<_RCLK);
  PORTD HIGH (1<<_RCLK);
}

void digit4_3(int n, int replay, bool showZero)
{
  int n1, n2, n3, n4;
  n1 = (int) n % 10;
  n2 = (int) ((n % 100)-n1)/10;
  n3 = (int) ((n % 1000) - n2 - n1) / 100;
  n4 = (int) ((n % 10000) - n3 - n2 - n1) / 1000;
  for(int i = 0; i<=replay; i++)
  {
    pre_send(_LED_TAB[n1], 0b0001);
    if(showZero | (n>9)) pre_send(_LED_TAB[n2], 0b0010);
    if(showZero | (n>99)) pre_send(_LED_TAB[n3], 0b0100);
    if(showZero | (n>999)) pre_send(_LED_TAB[n4], 0b1000);
  }
}

void digit4_2(int n, int replay)
{
  digit4_3(n, replay, false);
}

void digit4_1(int n)
{
  digit4_3(n, 0, false);
}

void digit4showZero_2(int n, int replay)
{
  digit4_3(n, replay, true);
}

void digit4showZero_1(int n)
{
  digit4_3(n, 0, true);
}

void digit2_3(int n, int value, int replay)
{
  int n1, n2;
  n1 = (int) n % 10;
  n2 = (int) ((n % 100)-n1)/10;
  for(int i = 0; i<=replay; i++)
  {
    pre_send(_LED_TAB[n1], value);
    pre_send(_LED_TAB[n2], value<<1);
  }
}

void digit2_2(int n, int value)
{
  digit2_3(n, value, 0);
}
