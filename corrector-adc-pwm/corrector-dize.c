/*

rm -f *.obj *.hex
avr-gcc -mmcu=atmega168 -Wall -g -Os -Werror -lm -mcall-prologues -std=c99 -o avr-pwm.obj corrector-dize.c; avr-objcopy -O ihex avr-pwm.obj avr-pwm.hex

avrdude -C avrdude.conf -q -p atmega328p -c arduino -P /dev/ttyUSB0 -b 57600 -D -U flash:w:avr-pwm.hex:i -D -U lock:w:0xFC:m

*/

#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
//#include "74HC595.c"

void init_pwm(void);
void init_adc(void);

//------------------0-----1-----2-----3-----4-----5-----6-----7-----8------9----dp
char SEGMENTE[] =
{
  0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x80
};

volatile unsigned char segcounter = 0;
volatile unsigned int display = 0;
volatile float dest_voltage = 0;
volatile unsigned int display_indicate = 0;
volatile unsigned long value;
volatile unsigned int adc_counter = 0;

// Прерывание по совпадению T2
ISR (TIMER2_COMPA_vect)
{
}

ISR (TIMER2_COMPB_vect)
{
}

// Прерывание по переполнению T2, динамическая индикация
ISR (TIMER2_OVF_vect)
{
//   PORTD = 0xFF; // Гасим все сегменты
//   PORTC = (2 << segcounter); // Последовательно зажигаем общие аноды
//   switch (segcounter)
//   {
//     // индикатор 1
//     case 0:
//     PORTD = ~((SEGMENTE[display % 1000 / 100])|0x80); // Выводим первый разряд, добавляем десятичную точку
//     break;
//     case 1:
//     PORTD = ~(SEGMENTE[display % 100 / 10]); // Выводим 2 разряд
//     break;
//     case 2:
//     PORTD = ~(SEGMENTE[display % 10 / 1]); // Выводим 3 разряд
//     break;
//     // индикатор 2
//     case 3:
//     PORTD = ~((SEGMENTE[display_indicate % 1000 / 100])|0x80); // Выводим первый разряд, добавляем десятичную точку
//     break;
//     case 4:
//     PORTD = ~(SEGMENTE[display_indicate % 100 / 10]); // Выводим 2 разряд
//     break;
//   }
//   if ((segcounter++) > 4) segcounter = 0;
}

// Прерывание по окончанию преобразования АЦП
ISR (ADC_vect)
{
  value += (ADCL|ADCH<<8); // Суммируем старое и новое значения АЦП, преобразуем
  //ADCL|ADCH<<2;ADMUX |= (1<<ADLAR);
  //ADCL|ADCH<<8;ADMUX |= (0<<ADLAR);

  adc_counter++; // Увеличиваем счетчик измерений
  if(adc_counter > 2) // Если количество измерений больше N
  {
    display = value/adc_counter/5.0; // Вычисляем среднее значение АЦП диапазон min 0, max 199.
    if(display>=255)display=255; //  overflow

    if(display) dest_voltage = display & 0x3ff;

    if(display>=0&&display<20) dest_voltage =  (0+display) & 0x3ff;

    if(display>=20&&display<22) dest_voltage = ((uint16_t)(1.02*display)) & 0x3ff;
    if(display>=22&&display<24) dest_voltage = ((uint16_t)(1.05*display)) & 0x3ff;
    if(display>=24&&display<26) dest_voltage = ((uint16_t)(1.08*display)) & 0x3ff;
    if(display>=26&&display<28) dest_voltage = ((uint16_t)(1.10*display)) & 0x3ff;
    if(display>=28&&display<30) dest_voltage = ((uint16_t)(1.10*display)) & 0x3ff;

    if(display>=30&&display<32) dest_voltage = ((uint16_t)(1.08*display)) & 0x3ff;
    if(display>=32&&display<34) dest_voltage = ((uint16_t)(1.05*display)) & 0x3ff;
    if(display>=34&&display<36) dest_voltage = ((uint16_t)(1.02*display)) & 0x3ff;
    if(display>=36&&display<38) dest_voltage = ((uint16_t)(1.01*display)) & 0x3ff;
    if(display>=38&&display<40) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;

    if(display>=40&&display<100) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;

    if(display>=100&&display<102) dest_voltage = ((uint16_t)(0.99*display)) & 0x3ff;
    if(display>=102&&display<104) dest_voltage = ((uint16_t)(0.98*display)) & 0x3ff;
    if(display>=104&&display<106) dest_voltage = ((uint16_t)(0.97*display)) & 0x3ff;
    if(display>=106&&display<108) dest_voltage = ((uint16_t)(0.96*display)) & 0x3ff;
    if(display>=108&&display<110) dest_voltage = ((uint16_t)(0.95*display)) & 0x3ff;

    if(display>=110&&display<112) dest_voltage = ((uint16_t)(0.94*display)) & 0x3ff;
    if(display>=112&&display<114) dest_voltage = ((uint16_t)(0.93*display)) & 0x3ff;
    if(display>=114&&display<116) dest_voltage = ((uint16_t)(0.92*display)) & 0x3ff;
    if(display>=116&&display<118) dest_voltage = ((uint16_t)(0.91*display)) & 0x3ff;
    if(display>=118&&display<120) dest_voltage = ((uint16_t)(0.90*display)) & 0x3ff;

    if(display>=120&&display<150) dest_voltage = ((uint16_t)(0.90*display)) & 0x3ff;

    if(display>=150&&display<152) dest_voltage = ((uint16_t)(0.91*display)) & 0x3ff;
    if(display>=152&&display<154) dest_voltage = ((uint16_t)(0.92*display)) & 0x3ff;
    if(display>=154&&display<156) dest_voltage = ((uint16_t)(0.93*display)) & 0x3ff;
    if(display>=156&&display<158) dest_voltage = ((uint16_t)(0.94*display)) & 0x3ff;
    if(display>=158&&display<160) dest_voltage = ((uint16_t)(0.95*display)) & 0x3ff;

    if(display>=160&&display<162) dest_voltage = ((uint16_t)(0.96*display)) & 0x3ff;
    if(display>=162&&display<164) dest_voltage = ((uint16_t)(0.97*display)) & 0x3ff;
    if(display>=164&&display<166) dest_voltage = ((uint16_t)(0.98*display)) & 0x3ff;
    if(display>=166&&display<168) dest_voltage = ((uint16_t)(0.99*display)) & 0x3ff;
    if(display>=168&&display<170) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;

    if(display>=170&&display<255) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;

    display = display*2.5;
    display_indicate=dest_voltage*2.5;
    //digit4_3(dest_voltage*22.0, 10, true);

    adc_counter = 0; // Обнуляем счетчик измерений
    value = 0;
    // Обнуляем буфер АЦП
    // ADCSRA |= (1 << ADIF); // interrupt enable прерывание по окончанию преобразования.
    // ADIF автоматом снимается при уходе на вектор прерывания по АЦП. 
    // ADCSRA |= (1 << ADSC); // старт преобразования
  }
}

// формирование выходного pwm сигнала
ISR(TIMER1_COMPA_vect)
{
  //OCR2=dest_voltage; // 8
  OCR2A=dest_voltage; // 88
  ADCSRA |= (1 << ADSC); // старт преобразования
}

// Главная функция
int main (void)
{
  init_pwm();
  init_adc();
  DDRC = 0b11111110; // set PortC anode
  PORTC = 0x00; // Ноль на выходе
  DDRD = 0xFF; // Выходы на сегменты
  PORTD = 0x00; // Ноль на выходе

  sei(); // Глобально разрешаем прерывания
  // Главный цикл
  while(1)
  {
  }
}

void init_adc(void)
{
  // Настройка АЦП
  ADCSRA |= (0 << ADPS2)|(0 << ADPS1)|(1 << ADPS0); // F_CPU 8000000/64=125kHz
  ADMUX  |= (0 << ADLAR);
  ADMUX  |= (0 << REFS1)|(1 << REFS0); // 01 - V питания (вход AREF должен быть отключен)
  ADMUX  |= (0 << MUX3); // усиления нет
  ADMUX  |= (0 << MUX2)|(0 << MUX1)|(0 << MUX0); // вход - ADC0
  ADCSRA |= 1 << ADSC;
  ADCSRA |= (1 << ADEN)|(1 << ADATE)|(1 << ADIE); // 88
  ACSR |= (0 << ACBG); // внутренний источник опорного напряжения 1.23 отключен
}

void init_pwm(void)
{
  DDRB=0b00001000; // portB pin3 как выход
  PORTB=0x00; // Ноль на выходе
  //FOC1A/B Force output compare A/B
  TCCR1C |= (0 << FOC1A)|(0 << FOC1B); // 88
  TCCR1A |= (0 << COM1A1)|(0 << COM1A0)|(0 << COM1B1)|(0 << COM1B0)|(0 << WGM11)|(0 << WGM10); // 8/88
  TCCR1B |= (0 << ICNC1)|(0 << ICES1)|(0 << WGM13)|(1 << WGM12)|(0 << CS12)|(0 << CS11)|(1 << CS10); // 8/88
  TCNT1H=0x00;// Timer/Counter1 – Counter Register High byte
  TCNT1L=0x00;// Timer/Counter1 – Counter Register Low byte
  ICR1=0x00; // Input Capture register
  OCR1AH=0x04; // Timer/Counter1 - output compare register A high byte
  OCR1AL=0xE2; // Timer/Counter1 - output compare register A low byte
  OCR1BH=0x00; // Timer/Counter1 - output compare register B high byte
  OCR1BL=0x00; // Timer/Counter1 - output compare register B low byte
  //ASSR |= (0 << AS2)|(0 << TCN2UB)|(0 << OCR2UB)|(0 << TCR2UB); // 8
  //ASSR |= (0 << EXCLK)|(0 << AS2)|(0 << TCN2UB)|(0 << OCR2AUB)|(0 << OCR2BUB)|(0 << TCR2AUB)|(0 << TCR2BUB); // 88
  ASSR |= (0 << AS2)|(0 << TCN2UB)|(0 << OCR2AUB)|(0 << OCR2BUB)|(0 << TCR2AUB)|(0 << TCR2BUB); // 88

  TCCR2B |= (0 << FOC2A)|(0 << FOC2B); // 88
  TCCR2B |= (0 << WGM22)|(0 << CS22)|(0 << CS21)|(1 << CS20); // 001 Предделитель/1
  TCCR2A |= (1 << COM2A1)|(0 << COM2A0)|(0 << COM2B1)|(0 << COM2B0)|(1 << WGM21)|(1 << WGM20); // 88

  TIMSK1 |= (1 << OCIE1A)|(1 << TOIE2); // 88
  TIMSK2 |= (1 << OCIE2B)|(1 << OCIE2A)|(1 << TOIE2); // 88

  sei();
}
