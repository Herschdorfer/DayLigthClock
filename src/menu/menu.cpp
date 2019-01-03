#include <Arduino.h>
#include "SunRiseClock.h"
#include "menu.h"
#include "lcd/lcd.h"
#include "serial.h"
#include "wifi_bt/wifi.h"

typedef char *(*statusFunctionPointer)();
typedef void (*actionFunctionPointer)();

class MenuItem
{
  public:
    MenuItem(const char *caption, statusFunctionPointer menuPtr, actionFunctionPointer actionFunctions, MenuItem *children, uint8_t numOfChildren)
    {
        this->caption = caption;
        this->statusfunction = menuPtr;
        this->actionFunctions = actionFunctions;
        this->children = children;
        this->numOfChildren = numOfChildren;
    }

    const char *caption;
    statusFunctionPointer statusfunction;
    actionFunctionPointer actionFunctions;
    MenuItem *children;
    uint8_t numOfChildren;
};

int bluetooth = 1;

char *Bluetooth_isEnabled()
{
    return (bluetooth == 1) ? (char *)"Enabled" : (char *)"Disabled";
}

char *EmptyChars()
{
    return (char *)"";
}

void nothing()
{
}

MenuItem bluetoothMenu = MenuItem("Bluetooth", &Bluetooth_isEnabled, &nothing, nullptr, 0);
MenuItem clockMenu = MenuItem("Clock", &EmptyChars, &nothing, nullptr, 0);
MenuItem WifiMenu = MenuItem("Wifi", &WIFI_getStatus, &WIFI_ToggleWifi, nullptr, 0);

MenuItem rootChildren[] = {bluetoothMenu, clockMenu, WifiMenu};
MenuItem root = MenuItem("MainMenu", &EmptyChars, &nothing, rootChildren, SIZE_OF(rootChildren, MenuItem));

static MenuItem *currentItem;
static uint8_t currentChild = 0;

void Menu_init()
{
    currentItem = &root;
}

void Menu_LeftButtonPressed(void)
{
    if (currentItem == &root)
    {
        LCD_activateHome();
    }
}

void Menu_CenterButtonPressed(void)
{
    if (currentItem->numOfChildren > 0)
    {
        currentChild = (currentChild + 1) % currentItem->numOfChildren;
        Serial_log("SM", "Current child moved to %d", currentChild);
        LCD_setDisplayBuffer((char *)(currentItem->children[currentChild].caption), ((*currentItem).children[currentChild].statusfunction)());
    }
}

void Menu_RightButtonPressed(void)
{
    if (LCD_getStatus() == HOME)
    {
        LCD_activateMenu();
        currentChild = 0;
        LCD_setDisplayBuffer((char *)(currentItem->children[currentChild].caption), ((*currentItem).children[currentChild].statusfunction)());
    }
    else
    {
        if (currentItem->children[currentChild].numOfChildren > 0)
        {
            currentItem = &currentItem->children[currentChild];
        }
        else
        {
            ((*currentItem).children[currentChild].actionFunctions)();
            LCD_setDisplayBuffer((char *)(currentItem->children[currentChild].caption), ((*currentItem).children[currentChild].statusfunction)());
        }
    }
}