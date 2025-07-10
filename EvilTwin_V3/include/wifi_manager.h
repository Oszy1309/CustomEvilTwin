#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <esp_wifi.h>
#include <Arduino.h>

// Externe Variablen (falls andere Dateien darauf zugreifen müssen)
extern wifi_ap_record_t ap_records[20];
extern uint16_t ap_count;
extern bool scanning;

// Funktionsdeklarationen
void init_wifi();
void start_scan();
void scan_loop();
void set_target(uint8_t index);
void capture_pmkid();

#endif