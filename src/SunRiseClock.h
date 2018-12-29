#pragma once
#include "Arduino.h"

#define SIZE_OF(var, type) sizeof(var) / sizeof(type)

void _log(const char *format, ...);