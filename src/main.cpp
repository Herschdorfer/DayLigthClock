#include <Arduino.h>
#include "SunRiseClock.h"
#include "sun/sun.h"
#include "button/button.h"
#include "lcd/lcd.h"
#include "clock/clock.h"
#include "serial.h"
#include "menu/menu.h"
#include "wifi_bt/wifi.h"

void setup()
{
  Serial_init();
  Serial_log("SY", "Sunrise Alarm Clock starting");

  Menu_init();
  Sun_init();
  LCD_init();
  Button_init();
  WIFI_init();
}

void loop()
{
  WIFI_handleOperations();
  Button_handleButton();
  LCD_updateDisplay();

  /*   RGBColor_t colors;

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
  } */
}
