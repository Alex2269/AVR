/*

avr-gcc -mmcu=atmega168 -Wall -g -Os -Werror -lm -mcall-prologues -std=c99 -o avr-pwm.obj corrector-7-seg-168.c; avr-objcopy -O ihex avr-pwm.obj avr-pwm.hex

avrdude -C avrdude.conf -q -p atmega328p -c arduino -P /dev/ttyUSB0 -b 57600 -D -U flash:w:avr-pwm.hex:i -D -U lock:w:0xFC:m

*/

#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
#include "74HC595.c"

void init_pwm(void);
void init_adc(void);

//------------------0-----1-----2-----3-----4-----5-----6-----7-----8------9----dp
char SEGMENTE[] =
{
  0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x80
};

volatile unsigned char segcounter = 0;
volatile unsigned int display = 0;
volatile unsigned int dest_voltage = 0;
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
  PORTD = 0xFF; // Гасим все сегменты
  PORTC = (2 << segcounter); // Последовательно зажигаем общие аноды
  switch (segcounter)
  {
    // индикатор 1
    case 0:
    PORTD = ~((SEGMENTE[display % 1000 / 100])|0x80); // Выводим первый разряд, добавляем десятичную точку
    break;
    case 1:
    PORTD = ~(SEGMENTE[display % 100 / 10]); // Выводим 2 разряд
    break;
    case 2:
    PORTD = ~(SEGMENTE[display % 10 / 1]); // Выводим 3 разряд
    break;
    // индикатор 2
    case 3:
    PORTD = ~((SEGMENTE[display_indicate % 1000 / 100])|0x80); // Выводим первый разряд, добавляем десятичную точку
    break;
    case 4:
    PORTD = ~(SEGMENTE[display_indicate % 100 / 10]); // Выводим 2 разряд
    break;
  }
  if ((segcounter++) > 4) segcounter = 0;
}

// Прерывание по окончанию преобразования АЦП
ISR (ADC_vect)
{
  value += (ADCL|ADCH<<8); // Суммируем старое и новое значения АЦП, преобразуем
  adc_counter++; // Увеличиваем счетчик измерений
  if(adc_counter > 1) // Если количество измерений больше N
  {
    display = value/adc_counter/5.0; // Вычисляем среднее значение АЦП диапазон min 0, max 199.

    if(display) dest_voltage = display & 0x3ff;

    if(display>=0&&display<40) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;

    if(display>=40&&display<42) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=42&&display<44) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=44&&display<46) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=46&&display<48) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=48&&display<50) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=50&&display<52) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=52&&display<54) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=54&&display<56) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=56&&display<58) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=58&&display<60) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;

    if(display>=60&&display<120) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;

    if(display>=120&&display<122) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=122&&display<124) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=124&&display<126) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=126&&display<128) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=128&&display<130) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=130&&display<132) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=132&&display<134) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=134&&display<136) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=136&&display<138) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=138&&display<140) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;

    if(display>=140&&display<142) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=142&&display<144) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=144&&display<146) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=146&&display<148) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=148&&display<150) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=150&&display<152) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=152&&display<154) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=154&&display<156) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=156&&display<158) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=158&&display<160) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;

    if(display>=160&&display<202) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=202&&display<204) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=204&&display<206) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=206&&display<208) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=208&&display<210) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=210&&display<212) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=212&&display<214) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=214&&display<216) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=216&&display<218) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=218&&display<220) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;

    if(display>=220&&display<222) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=222&&display<224) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=224&&display<226) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=226&&display<228) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=228&&display<230) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=230&&display<232) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=232&&display<234) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=234&&display<236) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=236&&display<238) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=238&&display<240) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;

    if(display>=240&&display<242) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=242&&display<244) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=244&&display<246) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=246&&display<248) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=248&&display<250) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=250&&display<252) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;
    if(display>=252&&display<255) dest_voltage = ((uint16_t)(1.00*display)) & 0x3ff;

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
  //TCCR2 |= (1 << CS21); // 8 Предделитель на 8
  TCCR2B |= (0 << CS22)|(0 << CS21)|(1 << CS20); // 88 :001 Предделитель/1
  sei(); // Глобально разрешаем прерывания
  // Главный цикл
  while(1)
  {
  }
}

void init_adc(void)
{
  // Настройка АЦП
  /*
  ADCSRA |=
  (1 << ADEN) // Разрешение АЦП
  |(1 << ADSC) // Запуск преобразования
  |(1 << ADFR) // Непрерывный режим работы АЦП
  |(1 << ADPS2)|(1 << ADPS1) // Предделитель на 64 (частота АЦП 125kHz)
  |(1 << ADIE); // Разрешение прерывания от АЦП
  */
  //ADCSRA |= (1 << ADEN)|(1 << ADSC)|(1 << ADFR)|(1 << ADPS2)|(1 << ADPS1)|(1 << ADIE); // 8
  ADCSRA |= (1 << ADEN)|(1 << ADSC)|(1 << ADATE)|(1 << ADIE); // 88
  ADCSRA |= (1 << ADPS2)|(1 << ADPS1)|(0 << ADPS0); // 88  F_CPU 8000000/64=125kHz
  ACSR |= (0 << ACBG); // внутренний источник опорного напряжения 1.23 отключен
  ADMUX = 0;
  ADMUX |= (0 << REFS1)|(1 << REFS0); // 01 - V питания (вход AREF должен быть отключен) с внешней емкостью AREF-GND
  //ADMUX |= (1 << REFS1)|(1 << REFS0); // внутреннее напряжение Vref=2,56 В с внешней емкостью AREF-GND
  //ADMUX |= (0 << REFS1)|(0 << REFS0); // выбираем внешний источник соединив VCC & AREF с внешней емкостью AREF-GND
  // Результат аналого-цифрового преобразования хранится в регистрах
  // ADCH:ADCL и может размещаться двумя способами
  // ADCH-2 бита ADCL 8бит ADCH-8 бит и ADCL 2бита ,
  // для выравнивания разультата используется регистр ADLAR
  // выравнивание ADLAR=0 – слева, ADLAR=1 – справа.
  ADMUX |= (0<<ADLAR);
  ADMUX |= (0 << MUX3); // усиления нет
  ADMUX |= (0 << MUX2)|(0 << MUX1)|(0 << MUX0); // вход - ADC0
}

void init_pwm(void)
{
  DDRB=0b00001000; // portB pin3 как выход
  PORTB=0x00; // Ноль на выходе
  //FOC1A/B Force output compare A/B
  //TCCR1A |= (0 << FOC1A)|(0 << FOC1B); // 8
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
  // TCCR0 |= (0 << CS02)|(0 << CS01)|(0 << CS00); // 8
  TCCR0B |= (0 << FOC0A)|(0 << FOC0B)|(0 << WGM02)|(0 << CS02)|(0 << CS01)|(0 << CS00); // 88
  TCCR0A |= (0 << COM0A1)|(0 << COM0A0)|(0 << COM0B1)|(0 << COM0B0)|(0 << WGM01)|(0 << WGM00); // 88
  // TCCR2 |= (0 << FOC2); // 8
  // TCCR2 |= (1 << WGM20)|(1 << COM21)|(0 << COM20)|(1 << WGM21)|(0 << CS22)|(1 << CS21)|(1 << CS20); // 8
  TCCR2B |= (0 << FOC2A)|(0 << FOC2B); // 88
  TCCR2B |= (0 << WGM22)|(0 << CS22)|(0 << CS21)|(0 << CS20); // 88
  TCCR2A |= (1 << COM2A1)|(0 << COM2A0)|(0 << COM2B1)|(0 << COM2B0)|(1 << WGM21)|(1 << WGM20); // 88
  //OCR2=0x00; // 8
  OCR2A=0x00; // 88
  //(1 << TOIE2); // Разрешение прерывания по таймеру2
  //TIMSK |= (0 << OCIE2)|(1 << TOIE2)|(0 << TICIE1)|(1 << OCIE1A)|(0 << OCIE1B)|(0 << TOIE1)|(0 << TOIE0); // 8
  //TIMSK0 |= (0 << OCIE0B)|(0 << OCIE0A)|(0 << TOIE0); // 88
  //TIMSK1 |= (0 << ICIE1)|(0 << OCIE1B)|(1 << OCIE1A)|(0 << TOIE1); // 88
  //TIMSK2 |= (0 << OCIE2B)|(0 << OCIE2A)|(1 << TOIE2); // 88
  TIMSK1 |= (1 << OCIE1A)|(1 << TOIE2); // 88
  TIMSK2 |= (1 << OCIE2B)|(1 << OCIE2A)|(1 << TOIE2); // 88
  sei();
}

// grep -i "ADC_vect" `find /usr/*/avr/*/ -type f -name '*8.h'`
// grep -i "TIMER1_COMPA_vect" `find /usr/*/avr/*/ -type f -name '*8.h'`
// grep -i "ADC_vect" `find /usr/*/avr/*/ -type f -name '*8.h'`
