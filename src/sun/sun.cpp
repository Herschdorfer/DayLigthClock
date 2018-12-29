#include "SunRiseClock.h"
#include "sun.h"

static Color_States_t _getCurrState(uint16_t step, Color_States_t * states, uint8_t numStates)
{
    // if the step is zero retun first step immideatly
    if (step == 0)
    {
        return states[0];
    }
    // if step is bigger max step return last step
    else if(step >= MAXSTEP)
    { 
        return states[numStates-1];
    }
    else
    {
        for (uint8_t i = numStates - 1; i >= 0 ; i--)
        {
            if (states[i].index == step)
            {
                return states[i];
            } 
            else if (states[i].index < step )
            {
                return states[i];
            }
        }
    }
    _log("ERROR: Invalid curret state retuned at step %d\r\n", step);
    return states[0];
}

static Color_States_t _getNextState(uint16_t step, Color_States_t * states, uint8_t numStates)
{
        // if the step is zero return the second step immideatly
    if (step == 0)
    {
        return states[1];
    }
    // if step is bigger than max step return last step
    else if(step >= MAXSTEP)
    { 
        return states[numStates-1];
    }
    else
    {
        for (uint8_t i = 0; i < numStates ; i++)
        {
            if (states[i].index >= step+1 )
            {
                return states[i];
            }
        }
    }
    _log("ERROR: Invalid next state retuned at step %d\n\r", step);
    return states[0];
}

static uint8_t _getCurrentColor(uint16_t step, Color_States_t * states, uint8_t numStates)
{
    Color_States_t currStep = _getCurrState(step, states, numStates);
    Color_States_t nextStep = _getNextState(step, states, numStates);

    if(currStep.index == nextStep.index)
    {
        return currStep.value;
    }
    else if(currStep.value == nextStep.value)
    {
        return currStep.value;
    }
    else if ( currStep.value < nextStep.value )
    {
        return (uint8_t)((float)step/(nextStep.index - currStep.index)*nextStep.value);
    }
    else
    {
        float v1 = (step - currStep.index);
        float v2 = (nextStep.index - currStep.index);
        float v3 = v1/v2;       
        return (uint16_t)( v3 *-(float)(currStep.value-nextStep.value))+(currStep.value);
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