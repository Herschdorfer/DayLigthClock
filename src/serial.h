#pragma once

void Serial_init(void);
void Serial_log(const char module[2], const char *format, ...);
void Serial_log(const char module[3], String string);

void Serial_debug(const char module[2], const char *format, ...);
void Serial_debug(const char module[3], String string);