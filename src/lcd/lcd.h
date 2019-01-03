
#pragma once

typedef enum LCD_States
{
    HOME,
    MENU,
} LCD_States_t;

void LCD_init(void);
void LCD_updateDisplay(void);
void LCD_setDisplayBuffer(char *line1, char *line2);
LCD_States_t LCD_getStatus(void);

void LCD_activateHome(void);
void LCD_activateMenu(void);
