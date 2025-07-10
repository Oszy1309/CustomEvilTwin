#ifndef UART_UTILS_H
#define UART_UTILS_H

#include <Arduino.h>

// Funktionsdeklarationen
void send_uart(const char* msg);
void send_uart(const String& msg);

#endif