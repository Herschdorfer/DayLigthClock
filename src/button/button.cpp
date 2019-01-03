
#include <Arduino.h>
#include "button.h"

portMUX_TYPE muxL = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE muxC = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE muxR = portMUX_INITIALIZER_UNLOCKED;

volatile int interruptCounterR = 0;
volatile int interruptCounterL = 0;
volatile int interruptCounterC = 0;

#define L_BUTTON_PIN 14
#define R_BUTTON_PIN 12
#define C_BUTTON_PIN 17

void Button_handleButton()
{
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
}

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

void Button_init()
{
    pinMode(L_BUTTON_PIN, INPUT_PULLUP);
    pinMode(C_BUTTON_PIN, INPUT_PULLUP);
    pinMode(R_BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(L_BUTTON_PIN), handleLeftButton, FALLING);
    attachInterrupt(digitalPinToInterrupt(C_BUTTON_PIN), handleCenterButton, FALLING);
    attachInterrupt(digitalPinToInterrupt(R_BUTTON_PIN), handleRigthButton, FALLING);
}