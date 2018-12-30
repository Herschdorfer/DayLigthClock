#define DEBUG

#include "Arduino.h"
#include "SunRiseClock.h"
#include "sun/sun.h"
#include <LiquidCrystal.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

#define LCD_EN 11
#define LCD_RS 12
#define LCD_D4 5
#define LCD_D5 4
#define LCD_D6 3
#define LCD_D7 2

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void setup()
{
  Serial.begin(115200);
  _log("Sunrise Alarm Clock starting");

  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  Sun_init();

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
}

void loop()
{
  RGBColor_t colors;

  for (uint16_t time = 0; time < SUN_MAXSTEP + 1; time++)
  {
    colors = Sun_SetSunriseStep(time);
    char lcd_buffer[16];
    lcd.setCursor(0, 1);
    sprintf(lcd_buffer, "r%3d g%3d b%3d", colors.red, colors.green, colors.blue);
    lcd.print(lcd_buffer);
  }

  Sun_SetSunOff();

  while (true)
  {
  }
}

void _log(const char *format, ...)
{
#ifdef DEBUG
  char buffer[256];
  va_list args;
  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);
  Serial.print(buffer);
#endif
}