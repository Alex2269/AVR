#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
#include "74HC595.c"

void init_pwm(void);
void init_adc(void);

volatile unsigned char segcounter = 0;
volatile unsigned int display = 0;
volatile unsigned int dest_voltage = 0;
volatile unsigned int display_indicate = 0;
volatile unsigned long value;
volatile unsigned int adc_counter = 0;
volatile float multiplier = 22.0;

volatile int button_1=0;
volatile int button_2=0;

// Прерывание по совпадению T2-A
ISR (TIMER2_COMPA_vect)
{
}

// Прерывание по совпадению T2-B
ISR (TIMER2_COMPB_vect)
{
}

// Прерывание по переполнению T2, динамическая индикация
ISR (TIMER2_OVF_vect)
{
}

// Прерывание по окончанию преобразования АЦП
ISR (ADC_vect)
{
  value += (ADCL|ADCH<<8); // Суммируем старое и новое значения АЦП, преобразуем
  adc_counter++; // Увеличиваем счетчик измерений
  if(adc_counter > 1) // Если количество измерений больше N
  {
    display = value/adc_counter/5.0; // Вычисляем среднее значение АЦП диапазон min 0, max 199.

    dest_voltage = display & 0x3ff;

    display_indicate=dest_voltage*2.5;

    adc_counter = 0; // Обнуляем счетчик измерений
    value = 0;

    //---------

//     DDRD &= ~(1<<PD2)|(1<<PD3);  // Кнопка к PDx - GND. PD2-button_1 PD3-button_2 
//     PORTD |= (1<<PD2)|(1<<PD3);  // подключить внутренний нагрузочный резистор.
    DDRD  |= 0xFF; // Выходы на сегменты
    PORTD |= 0b000001100; // Ноль на выходе

    if (bit_is_clear(PIND, 2)) button_1++;
    if (button_1>200) {button_1=0; multiplier=multiplier-0.2;}    // если кнопка нажата P1_1 более 200 циклов

    if (bit_is_clear(PIND, 3)) button_2++;
    if (button_2>200) {button_2=0; multiplier=multiplier+0.2;}    // если кнопка нажата P1_2 более 200 циклов
    if (multiplier<2) multiplier=22.0;
    if (multiplier>220) multiplier=22.0;

    //---------

    digit4_3(dest_voltage*multiplier, 10, true);
  }
}

// формирование выходного pwm сигнала
ISR(TIMER1_COMPA_vect)
{
}

// Главная функция
int main (void)
{
  init_adc();
  DDRC = 0b11111110; // set PortC anode
  PORTC = 0x00; // Ноль на выходе
  DDRD = 0xFF;  // Выходы на сегменты
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
  ADCSRA |= (1 << ADEN)|(1 << ADSC)|(1 << ADATE)|(1 << ADIE);
  ADCSRA |= (1 << ADPS2)|(1 << ADPS1)|(0 << ADPS0); // F_CPU 8000000/64=125kHz
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

/*

apt install:
gcc-avr
gdb-avr
avrdude
avr-libc

compile:
avr-gcc -mmcu=atmega168 -Wall -g -Os -Werror -lm -mcall-prologues -std=c99 -o voltmeter.obj voltmeter-328-led-tube-2.c; avr-objcopy -O ihex voltmeter.obj voltmeter.hex

flash:
avrdude -C avrdude.conf -q -p atmega328p -c arduino -P /dev/ttyUSB0 -b 57600 -D -U flash:w:voltmeter.hex:i

*/
