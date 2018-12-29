#define DEBUG

#include "Arduino.h"
#include "SunRiseClock.h"
#include <Adafruit_NeoPixel.h>
#include "sun/sun.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

#define NUMPIXELS 16
#define PIN 8

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second

void setup()
{
  Serial.begin(115200);

  _log("Sunrise Alarm Clock starting");

  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  pixels.begin(); // This initializes the NeoPixel library.
}

RED red;
GREEN green;
BLUE blue;

void loop()
{
  for (uint16_t time = 0; time < MAXSTEP; time++)
  {
    uint8_t red_c = red.getCurrentColor(time);
    uint8_t green_c = green.getCurrentColor(time);
    uint8_t blue_c = blue.getCurrentColor(time);

    _log("STEP %4d RED: %4d GREEN: %4d BLUE %4d \r\n", time, red_c, green_c, blue_c);

    for (int i = 0; i < NUMPIXELS; i++)
    {
      pixels.setPixelColor(i, red_c, green_c, blue_c);
      pixels.show();
    }
  }

  while (true)
  {
  };
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