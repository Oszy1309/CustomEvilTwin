#include "wifi_manager.h"
#include "config.h"
#include "target.h"
#include "comm_uart.h"  // Verwende comm_uart statt uart_utils
#include <esp_wifi.h>
#include <WiFi.h>
#include "esp_err.h"

// Globale Variablen
wifi_ap_record_t ap_records[20];  // Maximale Anzahl von APs
uint16_t ap_count = 0;
bool scanning = false;

// Scan-Konfiguration
wifi_scan_config_t scanConf = {
    .ssid = 0,
    .bssid = 0,
    .channel = 0,
    .show_hidden = true
};

// WiFi initialisieren
void init_wifi() {
    WiFi.mode(WIFI_MODE_STA);
    esp_wifi_start();
    send_uart("[+] WiFi initialized");
}

// Scan starten
void start_scan() {
    scanning = true;
    esp_wifi_scan_start(&scanConf, false);
    send_uart("[SCAN STARTED]");
}

// Scan-Loop
void scan_loop() {
    static uint32_t last_scan_start = 0;
    uint16_t ap_count_temp = 0;
    
    if (!scanning && millis() - last_scan_start >= SCAN_TIMEOUT_MS) {
        start_scan();
        last_scan_start = millis();
    }
    
    if (scanning) return;
    
    if (esp_wifi_scan_get_ap_num(&ap_count_temp) == ESP_OK && ap_count_temp > 0) {
        ap_count = ap_count_temp;
        if (ap_count > 20) ap_count = 20;  // Begrenzen auf Array-Größe
        
        esp_wifi_scan_get_ap_records(&ap_count, ap_records);
        scanning = false;
        send_uart("[SCAN COMPLETE]");
        
        for (int i = 0; i < ap_count; i++) {
            String output = String(i) + ": " + String((char*)ap_records[i].ssid);
            send_uart(output);
            String rssi_output = " RSSI: " + String(ap_records[i].rssi);
            send_uart(rssi_output);
        }
    }
}

// Target setzen (ruft target.cpp Funktion auf)
void set_target(uint8_t index) {
    if (index < ap_count) {
        String ssid_str = String((char*)ap_records[index].ssid);
        set_target(ssid_str);  // Ruft Funktion aus target.cpp auf
    } else {
        send_uart("[-] Invalid index for target");
    }
}

// PMKID Capture (Platzhalter)
void capture_pmkid() {
    send_uart("[+] PMKID capture started");
}