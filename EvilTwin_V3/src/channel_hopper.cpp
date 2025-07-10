#include "channel_hopper.h"
#include "config.h"
#include "comm_uart.h"  // Für send_uart
#include "esp_wifi.h"

void ChannelHopper::hopChannel() {
    if (millis() - last_hop_time >= HOP_INTERVAL) {
        current_channel = (current_channel % MAX_CHANNEL) + 1;
        esp_wifi_set_channel(current_channel, WIFI_SECOND_CHAN_NONE);
        send_uart("[CHANNEL SWITCHED TO " + String(current_channel) + "]");
        last_hop_time = millis();
    }
}

int ChannelHopper::getCurrentChannel() {
    return current_channel;
}