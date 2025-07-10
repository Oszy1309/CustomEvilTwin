#ifndef COMM_UART_H
#define COMM_UART_H

#include <Arduino.h>

void init_uart();
void send_uart(String msg);
void handle_uart();

#endif // COMM_UART_H
