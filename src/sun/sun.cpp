#include "SunRiseClock.h"
#include "sun.h"
#include <Adafruit_NeoPixel.h>

#define NUMPIXELS 16
#define PIN 8

Sun sun;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

void Sun_init(void)
{
    pixels.begin(); // This initializes the NeoPixel library.
}

RGBColor_t Sun_SetSunriseStep(uint16_t step)
{
    RGBColor_t colors = sun.getColor(step);
    _log("STEP %4d RED: %4d GREEN: %4d BLUE %4d \r\n", step, colors.red, colors.green, colors.blue);
    for (int i = 0; i < NUMPIXELS; i++)
    {
        pixels.setPixelColor(i, colors.red, colors.green, colors.blue);
    }
    pixels.show();

    return colors;
}

void Sun_SetSunOff(void)
{
    for (int i = 0; i < NUMPIXELS; i++)
    {
        pixels.setPixelColor(i, 0, 0, 0);
    }
    pixels.show();
}

RGBColor_t Sun::getColor(uint16_t step)
{
    RGBColor_t ret;

    ret.blue = blue.getCurrentColor(step);
    ret.green = green.getCurrentColor(step);
    ret.red = red.getCurrentColor(step);

    return ret;
}

static Color_States_t _getCurrState(uint16_t step, Color_States_t *states, uint8_t numStates)
{
    // if the step is zero retun first step immideatly
    if (step == 0)
    {
        return states[0];
    }
    // if step is bigger max step return last step
    else if (step >= SUN_MAXSTEP)
    {
        return states[numStates - 1];
    }
    else
    {
        for (uint8_t i = numStates - 1; i >= 0; i--)
        {
            if (states[i].index == step)
            {
                return states[i];
            }
            else if (states[i].index < step)
            {
                return states[i];
            }
        }
    }
    _log("ERROR: Invalid curret state retuned at step %d\r\n", step);
    return states[0];
}

static Color_States_t _getNextState(uint16_t step, Color_States_t *states, uint8_t numStates)
{
    // if the step is zero return the second step immideatly
    if (step == 0)
    {
        return states[1];
    }
    // if step is bigger than max step return last step
    else if (step >= SUN_MAXSTEP)
    {
        return states[numStates - 1];
    }
    else
    {
        for (uint8_t i = 0; i < numStates; i++)
        {
            if (states[i].index >= step + 1)
            {
                return states[i];
            }
        }
    }
    _log("ERROR: Invalid next state retuned at step %d\n\r", step);
    return states[0];
}

static uint8_t _getCurrentColor(uint16_t step, Color_States_t *states, uint8_t numStates)
{
    Color_States_t currStep = _getCurrState(step, states, numStates);
    Color_States_t nextStep = _getNextState(step, states, numStates);

    if (currStep.index == nextStep.index)
    {
        return currStep.value;
    }
    else if (currStep.value == nextStep.value)
    {
        return currStep.value;
    }
    else if (currStep.value < nextStep.value)
    {
        float v1 = (step - currStep.index);
        float v2 = (nextStep.index - currStep.index);
        float v3 = v1 / v2;

        return (uint8_t)(v3 * nextStep.value);
    }
    else
    {
        float v1 = (step - currStep.index);
        float v2 = (nextStep.index - currStep.index);
        float v3 = v1 / v2;
        return (uint8_t)(v3 * -(float)(currStep.value - nextStep.value)) + (currStep.value);
    }
}

uint8_t RED::getCurrentColor(uint16_t step)
{
    return _getCurrentColor(step, states, SIZE_OF(states, Color_States_t));
}

uint8_t GREEN::getCurrentColor(uint16_t step)
{
    return _getCurrentColor(step, states, SIZE_OF(states, Color_States_t));
}

uint8_t BLUE::getCurrentColor(uint16_t step)
{
    return _getCurrentColor(step, states, SIZE_OF(states, Color_States_t));
}