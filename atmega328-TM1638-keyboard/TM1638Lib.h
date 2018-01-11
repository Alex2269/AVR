/***********************************************************************
 port from arduino project to avr-gcc
 https://github.com/Play-Zone/TM1638Module
*************************************************************************/

#ifndef tm1638lib_h
#define tm1638lib_h

void TM1638Lib(int _DIO, int _CLK, int _STB);
void LedDisplay(unsigned char, unsigned  char, unsigned  char, unsigned  char, unsigned char, unsigned  char, unsigned  char, unsigned  char);
unsigned char ReadKey(void);

void TM1638_Write(unsigned char);
unsigned char TM1638_Read(void);
void Write_COM(unsigned char);
void Write_DATA(unsigned char, unsigned char);
extern int DIO;// = 8;
extern int CLK;// = 9;
extern int STB;// = 10;

#endif
