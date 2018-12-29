
#pragma once
#include "Arduino.h"
#include "types.h"
#include "SunRiseColor.h"

#define MINSTEP 0
#define MAXSTEP 1000

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