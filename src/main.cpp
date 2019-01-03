

#include "Arduino.h"
#include "SunRiseClock.h"
#include "sun/sun.h"
#include "button/button.h"
#include "lcd/lcd.h"
#include "clock/clock.h"

#if defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#include <WebServer.h>
#endif
#include <time.h>
#include <AutoConnect.h>

#if defined(ARDUINO_ARCH_ESP8266)
ESP8266WebServer Server;
#elif defined(ARDUINO_ARCH_ESP32)
WebServer Server;
#endif
AutoConnect Portal(Server);
AutoConnectConfig Config; // Enable autoReconnect supported on v0.9.4

#define TIMEZONE (3600 * 9)          // Tokyo
#define NTPServer1 "at.pool.ntp.org" // NICT japan.
#define NTPServer2 "europe.pool.ntp.org"

#define DEBUG

void rootPage()
{
  String content =
      "<html>"
      "<head>"
      "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
      "</head>"
      "<body>"
      "<h2 align=\"center\" style=\"color:blue;margin:20px;\">Hello, world</h2>"
      "<h3 align=\"center\" style=\"color:gray;margin:10px;\">{{DateTime}}</h3>"
      "<p style=\"padding-top:10px;text-align:center\">" AUTOCONNECT_LINK(COG_32) "</p>"
                                                                                  "</body>"
                                                                                  "</html>";

  content.replace("{{DateTime}}", Time_getDateTime());
  Server.send(200, "text/html", content);
}

void setup()
{
  Serial.begin(115200);
  _log("Sunrise Alarm Clock starting");

  Sun_init();
  LCD_init();
  Button_init();

  Server.on("/", rootPage);

  Config.autoReconnect = true;
  Portal.config(Config);

  if (Portal.begin())
  {
    _log("WiFi connected: %s", WiFi.localIP().toString());
    configTime(3600, 0, NTPServer1, NTPServer2);
    setenv("TZ", "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", 1);
  }
}

void loop()
{
  Portal.handleClient();
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