
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "lcd.h"
#include "clock/clock.h"

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

typedef enum
{
    HOME,
    MENU,
} LCD_States_t;

static LCD_States_t currentState = HOME;
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

static void printHome(void);

void LCD_init()
{
    lcd.begin(16, 2);
}

void LCD_updateDisplay()
{

    switch (currentState)
    {
    case HOME:
        printHome();
        break;

    default:
        break;
    }
}

static void printHome()
{

    String date = Time_getDate();
    String time = Time_getTime();

    lcd.setCursor(0, 0);
    lcd.print(date);

    lcd.setCursor(0, 1);
    lcd.print(time);
}