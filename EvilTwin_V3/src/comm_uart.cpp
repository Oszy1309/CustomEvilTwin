#include <Arduino.h>
#include "config.h"
#include "target.h"
#include "wifi_manager.h"
#include "deauth.h"

void init_uart() {
    Serial.begin(UART_BAUD, SERIAL_8N1, FLIPPER_RX, FLIPPER_TX);
}

void send_uart(String msg) {
    Serial.println(msg);
}

void handle_uart() {
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        if (cmd == "START_SCAN") start_scan();
        else if (cmd.startsWith("SET_TARGET")) set_target(cmd.substring(10));
        else if (cmd == "DEAUTH_START") start_deauth();
        else if (cmd == "DEAUTH_STOP") stop_deauth();
    }
}
