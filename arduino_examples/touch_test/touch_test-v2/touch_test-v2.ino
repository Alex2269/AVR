// UTFT_Demo_480x320 (C)2013 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// This program is a demo of how to use most of the functions
// of the library with a supported display modules.
//
// This demo was made for modules with a screen resolution
// of 480x320 pixels.
//
// This program requires the UTFT library.
//

#include <Adafruit_GFX.h>    // Core graphics library
#include <UTFTGLUE.h>
#include <TouchScreen.h>

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE

uint16_t var_time;
uint32_t var_impuls;
uint32_t var_seconds;

#define ImpulsPin 11

UTFTGLUE tft(0x9488, A2, A1, A3, A4, A0);

#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define XS 480
#define YS 320

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 250 ohms across the X plate
#define YP A1 // must be an analog pin, use "An" notation!
#define XM A2 // must be an analog pin, use "An" notation!
#define YM 7 // can be a digital pin
#define XP 6 // can be a digital pin
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 550);

// Declare which fonts we will be using
#if !defined(SmallFont)
extern uint8_t SmallFont[];    //.kbv GLUE defines as GFXFont ref
#endif

void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(5));   //.kbv Due does not like A0
  pinMode(A0, OUTPUT);       //.kbv mcufriend have RD on A0
  digitalWrite(A0, HIGH);

  // Setup the LCD
  tft.InitLCD();
  tft.setRotation(1);
  tft.clrScr();
  tft.setFont(BigFont);
  tft.setTextSize(2);
}

void loop(void)
{
  menu();
}

void menu_display(void)
{
  tft.setColor(YELLOW);
  //  tft.fillRect(20, 10, 30, 40);
  tft.fillRect(10, 20, 40, 30);

  tft.fillRect(70, 10, 75, 40);
  tft.fillRect(80, 15, 85, 35);
  tft.fillRect(90, 20, 95, 30);
  tft.fillRect(100, 22, 105, 28);
  tft.fillCircle(160, 25, 10);

  tft.fillRect(XS - 70, 10, XS - 75, 40);
  tft.fillRect(XS - 80, 15, XS - 85, 35);
  tft.fillRect(XS - 90, 20, XS - 95, 30);
  tft.fillRect(XS - 100, 22, XS - 105, 28);
  tft.fillCircle(XS - 160, 25, 10);

  tft.fillRect(XS - 30, 10, XS - 20, 40);
  tft.fillRect(XS - 40, 20, XS - 10, 30);

  tft.setColor(BLUE);
  //  tft.fillRect(20, YS-40, 30, YS-10);
  tft.fillRect(10, YS - 30, 40, YS - 20);

  tft.fillRect(XS - 30, YS - 40, XS - 20, YS - 10);
  tft.fillRect(XS - 40, YS - 30, XS - 10, YS - 20);

  tft.fillRect(70, YS - 40, 75, YS - 10);
  tft.fillRect(80, YS - 35, 85, YS - 15);
  tft.fillRect(90, YS - 30, 95, YS - 20);
  tft.fillRect(100, YS - 28, 105, YS - 22);
  tft.fillCircle(160, YS - 25, 10);

  tft.fillRect(XS - 70, YS - 40, XS - 75, YS - 10);
  tft.fillRect(XS - 80, YS - 35, XS - 85, YS - 15);
  tft.fillRect(XS - 90, YS - 30, XS - 95, YS - 20);
  tft.fillRect(XS - 100, YS - 28, XS - 105, YS - 22);
  tft.fillCircle(XS - 160, YS - 25, 10);

  tft.setColor(GREEN);
  tft.fillCircle(35, YS / 2, 25); // run
  tft.fillCircle(XS - 35, YS / 2, 25); // stop
}

void update_display(void)
{
  tft.setColor(YELLOW);
  tft.printNumI(var_time, XS / 2 - 50, 15, 0);
  tft.setTextSize(2);
  tft.print("ms TIME", XS / 2 - 50, 45);
  tft.setColor(BLUE);
  tft.setTextSize(2);
  tft.printNumI(var_impuls, XS / 2 - 50, YS - 35, 0);
  tft.setTextSize(2);
  tft.print("IMPULS", XS / 2 - 50, YS - 65);
  tft.setTextSize(2);
  //
  var_seconds = (var_time * var_impuls) / 1000;
  tft.printNumI(var_seconds, XS / 2 - 50, YS / 2 + 35);
  tft.print("sec", XS / 2 + 50, YS / 2 + 35);
}

void label_stoped(void)
{
  tft.setColor(YELLOW);
  tft.print(" DONE ", XS / 2 - 50, YS / 2 - 15);
}

void menu(void)
{
  // a point object holds x y and z coordinates
  TSPoint p = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  pinMode(XP, OUTPUT);
  pinMode(YM, OUTPUT);
  if ( (p.x > 160 && p.x < 300) && (p.y > 160 && p.y < 300) )
  {
    Serial.print(" minus yellow ");
    var_time -= 50;
    if (var_time < 1 || var_time > 1000) var_time = 1;
    tft.clrScr();
  }
  if ( (p.x > 320 && p.x < 460) && (p.y > 160 && p.y < 300) )
  {
    Serial.print(" minus yellow ");
    var_time--;
    if (var_time < 1 || var_time > 1000) var_time = 1;
    tft.clrScr();
  }
  if ( (p.x > 600 && p.x < 740) && (p.y > 160 && p.y < 300) )
  {
    Serial.print(" plus yellow ");
    var_time++;
    if (var_time < 1 || var_time > 100) var_time = 100;
    tft.clrScr();
  }
  if ( (p.x > 780 && p.x < 1000) && (p.y > 160 && p.y < 300) )
  {
    Serial.print(" plus yellow ");
    var_time += 50;
    if (var_time < 1 || var_time > 100) var_time = 100;
    tft.clrScr();
  }
  if ( (p.x > 160 && p.x < 300) && (p.y > 800 && p.y < 1000) )
  {
    Serial.print(" minus blue ");
    var_impuls -= 5000;
    if (var_impuls < 1 || var_impuls > 100000) var_impuls = 0;
    tft.clrScr();
  }
  if ( (p.x > 780 && p.x < 1000) && (p.y > 800 && p.y < 1000) )
  {
    Serial.print(" plus blue ");
    var_impuls += 5000;
    if (var_impuls < 1 || var_impuls > 100000) var_impuls = 100000;
    tft.clrScr();
  }
  if ( (p.x > 320 && p.x < 460) && (p.y > 800 && p.y < 1000) )
  {
    Serial.print(" minus blue ");
    var_impuls -= 50;
    if (var_impuls < 1 || var_impuls > 100000) var_impuls = 0;
    tft.clrScr();
  }
  if ( (p.x > 600 && p.x < 740) && (p.y > 800 && p.y < 1000) )
  {
    Serial.print(" plus blue ");
    var_impuls += 50;
    tft.clrScr();
  }
  if (((p.x > 100 && p.x < 300) || (p.x > 800 && p.x < 1000)) && (p.y > 400 && p.y < 600))
  {
    Serial.print(" runing green ");
    uint8_t impuls1, impuls2, time1, time2;
    impuls1 = var_impuls % 256;
    impuls2 = var_impuls / 256;
    time1 = var_time % 256;
    time2 = var_time / 256;
    Serial.print(170); Serial.print(impuls1); Serial.print(impuls2); Serial.print(time1); Serial.print(time2);
    impuls (var_impuls , var_time);
    tft.clrScr();
  }
  update_display();
  menu_display();
  label_stoped();
}

void impuls ( unsigned int kol, unsigned int my_delay)
{
  pinMode(ImpulsPin, OUTPUT);
  digitalWrite(ImpulsPin, LOW);
  tft.clrScr();
  update_display();
  menu_display();
  tft.setColor(GREEN);
  tft.print(" RUNING ", XS / 2 - 40, YS / 2 - 15);
  tft.setColor(RED);
  tft.fillCircle(35, YS / 2, 25); // stop burron 1
  tft.fillCircle(XS - 35, YS / 2, 25); // stop button2
  for (int i; i < kol; i++)
  {
    digitalWrite(ImpulsPin, HIGH);
    delay(my_delay);
    digitalWrite(ImpulsPin, LOW);
    delay(my_delay);
    TSPoint p = ts.getPoint();
    if (((p.x > 100 && p.x < 300) || (p.x > 800 && p.x < 1000)) && (p.y > 400 && p.y < 600))
    {
      Serial.print(" stop red ");
      Serial.print(170); Serial.print(0); Serial.print(0); Serial.print(0); Serial.print(0);
      break;
    }
  }
  digitalWrite(ImpulsPin, LOW);
  tft.InitLCD();
  tft.setRotation(1);
}

