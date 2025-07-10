#ifndef TARGET_H
#define TARGET_H

#include <Arduino.h>

// Struktur für Target-Informationen
struct TargetAP {
    String ssid;
    uint8_t bssid[6];
    int rssi;
    uint8_t channel;
    bool selected;
};

extern TargetAP current_target;
extern bool deauthing;

// Funktionsdeklarationen
void set_target(String ssid);
void start_deauth();
void stop_deauth();

#endif