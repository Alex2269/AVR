//Demo for 2.0" TFT LCD
#include <stdint.h>
#include <UTFT.h>
#include <TouchScreen.h>
// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];
// GND -- GND
// 3V3 -- 3.3V
// CS -- A3
// RS -- A2
// WR -- A1
// RD -- 3.3V
// RST -- A0
// LED -- GND
// DB0 -- 8
// DB1 -- 9
// DB2 -- 10
// DB3 -- 11
// DB4 -- 4
// DB5 -- 13
// DB6 -- 6
// DB7 -- 7
// These are the pins for the touch pannel!
#define YP A1 // must be an analog pin, use "An" notation!
#define XM A2 // must be an analog pin, use "An" notation!
#define YM 7 // can be a digital pin
#define XP 6 // can be a digital pin
#define TS_MINX 175
#define TS_MINY 120
#define TS_MAXX 915
#define TS_MAXY 900 //you can debug to get the value
#define DEBUG_TS_VALUE 1
#define MINPRESSURE 10
#define MAXPRESSURE 1024
#define FRAME_WIDTH 10
#define RECT_WIDTH 100
#define RECT_HEIGHT 80
// Assign human-readable names to some common 16-bit color values:ST
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
// Remember to change the model parameter to suit your display module!
#define BOXSIZE 44
#define PENRADIUS 3
int oldcolor, currentcolor;
#define MINPRESSURE 10
#define MAXPRESSURE 1000
// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 250 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 550);
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_RS A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RST A0 //
#define ImpulsPin 2
uint16_t var_time;
uint16_t var_impuls;
UTFT tft(ST7775,LCD_RS,LCD_WR,LCD_CS,LCD_RST);
void setup(void)
{
  Serial.begin(9600);
  tft.InitLCD();
  tft.setContrast(2);
}

void loop(void)
{
  menu();
}

void menu_display(void)
{
  tft.fillRect(0, 22, 40, 10, YELLOW); //minus
  tft.fillRect(195, 5, 10, 45, YELLOW); // plus/2
  tft.fillRect(180, 22, 45, 10, YELLOW); // plus/2
  tft.fillRect(0, 142, 40, 10, BLUE); //minus
  tft.fillRect(195, 125, 10, 45, BLUE); // plus/2
  tft.fillRect(180, 142, 45, 10, BLUE); // plus/2
  tft.setColor(GREEN);
  tft.fillCircle(18, 85, 15); // run
  tft.fillCircle(195, 85, 15); // stop
}

void update_display(void)
{
  tft.setFont(BigFont);
  tft.setColor(YELLOW);
  tft.printNumI(var_time, 70, 280,0);
  tft.setColor(BLUE);
  tft.printNumI(var_impuls, 70, 390,0);
  tft.setColor(YELLOW);
  tft.print(" TIME", 55, 5);
  tft.setColor(BLUE);
  tft.print(" IMPULS", CENTER, 155);
  // tft.setFont(SevenSegNumFont);
  // tft.setColor(YELLOW);
  // tft.printNumI(var_time, 45, 280,0);
  // tft.setColor(BLUE);
  // tft.printNumI(var_impuls, 30, 360,0);
}

void stoped_display(void)
{
  tft.setColor(YELLOW);
  tft.setFont(BigFont);
  tft.print(" DONE ", CENTER, 80);
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
    var_time-=50;
    if(var_time<1||var_time>1000) var_time =1;
    tft.clrScr();
  }
  if ( (p.x > 320 && p.x < 460) && (p.y > 160 && p.y < 300) )
  {
    Serial.print(" minus yellow ");
    var_time--;
    if(var_time<1||var_time>1000) var_time =1;
    tft.clrScr();
  }
  if ( (p.x > 500 && p.x < 740) && (p.y > 160 && p.y < 300) )
  {
    Serial.print(" plus yellow ");
    var_time++;
    if(var_time<1||var_time>100) var_time =100;
    tft.clrScr();
  }
  if ( (p.x > 780 && p.x < 920) && (p.y > 160 && p.y < 300) )
  {
    Serial.print(" plus yellow ");
    var_time+=50;
    if(var_time<1||var_time>100) var_time =100;
    tft.clrScr();
  }
  if ( (p.x > 160 && p.x < 300) && (p.y > 700 && p.y < 840) )
  {
    Serial.print(" minus blue ");
    var_impuls-=1000;
    if(var_impuls<1||var_impuls>10000) var_impuls =0;
    tft.clrScr();
  }
  if ( (p.x > 780 && p.x < 920) && (p.y > 700 && p.y < 840) )
  {
    Serial.print(" plus blue ");
    var_impuls+=1000;
    if(var_impuls<1||var_impuls>10000) var_impuls =10000;
    tft.clrScr();
  }
  if ( (p.x > 320 && p.x < 460) && (p.y > 700 && p.y < 840) )
  {
    Serial.print(" minus blue ");
    var_impuls--;
    tft.clrScr();
  }
  if ( (p.x > 500 && p.x < 740) && (p.y > 700 && p.y < 840) )
  {
    Serial.print(" plus blue ");
    var_impuls++;
    tft.clrScr();
  }
  if (((p.x > 200 && p.x < 300)||(p.x > 800 && p.x < 900)) && (p.y > 450 && p.y < 550))
  {
    Serial.print(" runing green ");
    uint8_t impuls1, impuls2, time1, time2;
    impuls1 = var_impuls % 256;
    impuls2 = var_impuls / 256;
    time1 = var_time % 256;
    time2 = var_time / 256;
    Serial.print(170); Serial.print(impuls1);Serial.print(impuls2);Serial.print(time1);Serial.print(time2);
    impuls (var_impuls , var_time);
    tft.clrScr();
  }
  update_display();
  menu_display();
  stoped_display();
}

int impuls ( unsigned int kol, unsigned int my_delay)
{
  pinMode(ImpulsPin, OUTPUT);
  digitalWrite(ImpulsPin, LOW);
  tft.clrScr();
  update_display();
  menu_display();
  tft.setColor(GREEN);
  tft.setFont(BigFont);
  tft.print(" RUNING", CENTER, 80);
  tft.setColor(RED);
  tft.fillCircle(18, 85, 15); // stop burron 1
  tft.fillCircle(195, 85, 15); // stop button2
  for (int i;i<kol;i++)
  {
    digitalWrite(ImpulsPin, HIGH);
    delay(my_delay);
    digitalWrite(ImpulsPin, LOW);
    delay(my_delay);
    TSPoint p = ts.getPoint();
    if (((p.x > 200 && p.x < 300)||(p.x > 800 && p.x < 900)) && (p.y > 450 && p.y < 550))
    {
      Serial.print(" stop red ");
      Serial.print(170); Serial.print(0);Serial.print(0);Serial.print(0);Serial.print(0);
      break;
    }
  }
  digitalWrite(ImpulsPin, LOW);
  return;
}

