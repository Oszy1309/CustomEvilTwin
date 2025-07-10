#include "target.h"
#include "wifi_manager.h"
#include "comm_uart.h"

TargetAP current_target;
bool deauthing = false;

void set_target(String ssid) {
    for (int i = 0; i < ap_count; i++) {
        if (ssid == (char*)ap_records[i].ssid) {
            memcpy(current_target.bssid, ap_records[i].bssid, 6);
            current_target.ssid = String((char*)ap_records[i].ssid);
            current_target.rssi = ap_records[i].rssi;
            current_target.channel = ap_records[i].primary;
            current_target.selected = true;

            send_uart("[TARGET SET] " + ssid);
            return;
        }
    }
    send_uart("[ERROR] SSID not found");
}

void start_deauth() {
    deauthing = true;
    send_uart("[+] Deauth started");
}

void stop_deauth() {
    deauthing = false;
    send_uart("[+] Deauth stopped");
}
