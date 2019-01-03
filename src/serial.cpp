#include <Arduino.h>
#include "SunRiseClock.h"
#include "serial.h"

#define DEBUG

void Serial_init(void)
{
    Serial.begin(115200);
}

void Serial_log(const char module[3], const char *format, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    Serial.print("[");
    Serial.print(module);
    Serial.print("] ");
    Serial.println(buffer);
}

void Serial_log(const char module[3], String string)
{
    Serial.print("[");
    Serial.print(module);
    Serial.print("] ");
    Serial.println(string);
}

void Serial_debug(const char module[3], const char *format, ...)
{
#ifdef DEBUG
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    Serial.print("[");
    Serial.print(module);
    Serial.print("] ");
    Serial.println(buffer);
#endif
}

void Serial_debug(const char module[3], String string)
{
#ifdef DEBUG
    Serial.print("[");
    Serial.print(module);
    Serial.print("] ");
    Serial.println(string);
#endif
}