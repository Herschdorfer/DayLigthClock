

#include "Arduino.h"
#include "SunRiseClock.h"
#include "sun/sun.h"
#include <LiquidCrystal.h>

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

#if defined(UNO)
#define LCD_EN 11
#define LCD_RS 12
#define LCD_D4 5
#define LCD_D5 4
#define LCD_D6 3
#define LCD_D7 2
#elif defined(WEMOSBAT)
#define LCD_EN 22
#define LCD_RS 23
#define LCD_D4 5
#define LCD_D5 18
#define LCD_D6 19
#define LCD_D7 21
#endif

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

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
  static const char *wd[7] = {"Sun", "Mon", "Tue", "Wed", "Thr", "Fri", "Sat"};
  struct tm *tm;
  time_t t;
  char dateTime[26];

  t = time(NULL);
  tm = localtime(&t);
  sprintf(dateTime, "%04d/%02d/%02d(%s) %02d:%02d:%02d.",
          tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
          wd[tm->tm_wday],
          tm->tm_hour, tm->tm_min, tm->tm_sec);
  content.replace("{{DateTime}}", String(dateTime));
  Server.send(200, "text/html", content);
}

portMUX_TYPE muxL = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE muxC = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE muxR = portMUX_INITIALIZER_UNLOCKED;

volatile int interruptCounterR = 0;
volatile int interruptCounterL = 0;
volatile int interruptCounterC = 0;

void IRAM_ATTR handleLeftButton()
{
  portENTER_CRITICAL_ISR(&muxL);
  interruptCounterL++;
  portEXIT_CRITICAL_ISR(&muxL);
}

void IRAM_ATTR handleCenterButton()
{
  portENTER_CRITICAL_ISR(&muxC);
  interruptCounterC++;
  portEXIT_CRITICAL_ISR(&muxC);
}

void IRAM_ATTR handleRigthButton()
{
  portENTER_CRITICAL_ISR(&muxR);
  interruptCounterR++;
  portEXIT_CRITICAL_ISR(&muxR);
}

#define L_BUTTON_PIN 14
#define R_BUTTON_PIN 12
#define C_BUTTON_PIN 17

void setup()
{
  Serial.begin(115200);
  _log("Sunrise Alarm Clock starting");

  Sun_init();

  
  pinMode(L_BUTTON_PIN, INPUT_PULLUP);
  pinMode(C_BUTTON_PIN, INPUT_PULLUP);
  pinMode(R_BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(L_BUTTON_PIN), handleLeftButton, FALLING);
  attachInterrupt(digitalPinToInterrupt(C_BUTTON_PIN), handleCenterButton, FALLING);
  attachInterrupt(digitalPinToInterrupt(R_BUTTON_PIN), handleRigthButton, FALLING);

  delay(1000);

  // Behavior a root path of ESP8266WebServer.
  Server.on("/", rootPage);

  // Enable saved past credential by autoReconnect option,
  // even once it is disconnected.
  Config.autoReconnect = true;
  Portal.config(Config);

  // Establish a connection with an autoReconnect option.
  if (Portal.begin())
  {
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
    configTime(3600, 0, NTPServer1, NTPServer2);
    setenv("TZ", "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", 1);
  }

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
}

void loop()
{
  Portal.handleClient();

  if (interruptCounterR > 0)
  {
    portENTER_CRITICAL_ISR(&muxL);
    interruptCounterR--;
    portEXIT_CRITICAL_ISR(&muxL);

    Serial.println("right");
  }
  if (interruptCounterC > 0)
  {
    portENTER_CRITICAL_ISR(&muxC);
    interruptCounterC--;
    portEXIT_CRITICAL_ISR(&muxC);

    Serial.println("center");
  }
  if (interruptCounterL > 0)
  {
    portENTER_CRITICAL_ISR(&muxR);
    interruptCounterL--;
    portEXIT_CRITICAL_ISR(&muxR);

    Serial.println("left");
  }

  static const char *wd[7] = {"Sun", "Mon", "Tue", "Wed", "Thr", "Fri", "Sat"};
  struct tm *tm;
  time_t t;
  char dateTime[26];

  t = time(NULL);
  tm = localtime(&t);
  sprintf(dateTime, "%04d/%02d/%02d(%s)",
          tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
          wd[tm->tm_wday]);
  lcd.setCursor(0, 0);
  lcd.print(dateTime);

  sprintf(dateTime, "%02d:%02d:%02d",
          tm->tm_hour, tm->tm_min, tm->tm_sec);

  lcd.setCursor(0, 1);
  lcd.print(dateTime);

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