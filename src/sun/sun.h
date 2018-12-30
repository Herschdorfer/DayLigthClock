
#pragma once
#include "Arduino.h"
#include "types.h"
#include "SunRiseColor.h"

#define SUN_MAXSTEP 1000

/**
 * @brief Disables the all LEDs.
 * 
 */
void Sun_SetSunOff(void);

/**
 * @brief Sets the corresponding colors in the Neopixel library and initiates the transfer.
 * 
 * @param step          The current time step between 0 and @ref SUN_MAXSTEP.
 * @return RGBColor_t   The current color settings of the step.
 */
RGBColor_t Sun_SetSunriseStep(uint16_t step);

/**
 * @brief initialises the Neopixel library.
 * 
 */
void Sun_init(void);

class RED
{
public:
  uint8_t getCurrentColor(uint16_t step);

private:
  Color_States_t states[4] = {
      {0, 0},
      {100, 255},
      {333, 255},
      {1000, 128},
  };
};

class GREEN
{
public:
  uint8_t getCurrentColor(uint16_t step);

private:
  Color_States_t states[5] = {
      {0, 0},
      {100, 0},
      {333, 150},
      {933, 150},
      {1000, 128},
  };
};

class BLUE
{
public:
  uint8_t getCurrentColor(uint16_t step);

private:
  Color_States_t states[3] = {
      {0, 0},
      {333, 0},
      {1000, 255},
  };
};

class Sun
{
public:
  RGBColor_t getColor(uint16_t step);

private:
  RED red;
  GREEN green;
  BLUE blue;
};
