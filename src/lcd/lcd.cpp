#include <Arduino.h>
#include <LiquidCrystal.h>
#include "lcd.h"
#include "clock/clock.h"
#include "serial.h"

#if defined(UNO)
#define LCD_EN 11
#define LCD_RS 12
#define LCD_D4 5
#define LCD_D5 4
#define LCD_D6 3
#define LCD_D7 2
#elif defined(ESP32)
#define LCD_EN 22
#define LCD_RS 23
#define LCD_D4 5
#define LCD_D5 18
#define LCD_D6 19
#define LCD_D7 21
#endif

typedef struct
{
    char *lineOneBuffer[16];
    char *lineTwoBuffer[16];
} LCD_Data_t;

static LCD_States_t currentState = HOME;
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

char line1[16] = "";
char line2[16] = "";

static void printHome(void);
static void printMenu(void);

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

    case MENU:
        printMenu();
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

static void printMenu()
{

    lcd.setCursor(0, 0);
    lcd.print(line1);

    lcd.setCursor(0, 1);
    lcd.print(line2);
}

void LCD_setDisplayBuffer(char newLine1[16], char newLine2[16])
{
    lcd.clear();
    memcpy(line1, newLine1, 16);
    memcpy(line2, newLine2, 16);
}

void LCD_activateHome(void)
{
    currentState = HOME;
    Serial_log("LC", "switched to HOME mode");
}

void LCD_activateMenu(void)
{
    currentState = MENU;
    Serial_log("LC", "switched to Menu mode");
}

LCD_States_t LCD_getStatus()
{
    return currentState;
}