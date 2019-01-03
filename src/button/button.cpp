#include <Arduino.h>
#include "button.h"
#include "menu/menu.h"
#include "serial.h"

portMUX_TYPE muxL = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE muxC = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE muxR = portMUX_INITIALIZER_UNLOCKED;

volatile boolean interruptCounterR = false;
volatile boolean interruptCounterL = false;
volatile boolean interruptCounterC = false;

#define L_BUTTON_PIN 14
#define R_BUTTON_PIN 12
#define C_BUTTON_PIN 17

void Button_handleButton()
{
    if (interruptCounterR)
    {
        Serial_debug("BT", "RIGTH button Press dectected.");
        portENTER_CRITICAL_ISR(&muxL);
        interruptCounterR = false;
        portEXIT_CRITICAL_ISR(&muxL);

        Menu_RightButtonPressed();
    }
    if (interruptCounterC)
    {
        Serial_debug("BT", "CENTER button Press dectected.");
        portENTER_CRITICAL_ISR(&muxC);
        interruptCounterC = false;
        portEXIT_CRITICAL_ISR(&muxC);

        Menu_CenterButtonPressed();
    }
    if (interruptCounterL)
    {
        Serial_debug("BT", "LEFT button Press dectected.");
        portENTER_CRITICAL_ISR(&muxR);
        interruptCounterL = false;
        portEXIT_CRITICAL_ISR(&muxR);

        Menu_LeftButtonPressed();
    }
}

void IRAM_ATTR handleLeftButton()
{
    portENTER_CRITICAL_ISR(&muxL);
    interruptCounterL = true;
    portEXIT_CRITICAL_ISR(&muxL);
}

void IRAM_ATTR handleCenterButton()
{
    portENTER_CRITICAL_ISR(&muxC);
    interruptCounterC = true;
    portEXIT_CRITICAL_ISR(&muxC);
}

void IRAM_ATTR handleRigthButton()
{
    portENTER_CRITICAL_ISR(&muxR);
    interruptCounterR = true;
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