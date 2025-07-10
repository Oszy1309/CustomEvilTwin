#include <Arduino.h>
#include "config.h"
#include "comm_uart.h"
#include "wifi_manager.h"
#include "web_server.h"
#include "sd_logger.h"
#include "target.h"
#include "deauth.h"
#include "channel_hopper.h"
#include "pmkid_capture.h"

ChannelHopper hopper;

void setup() {
    Serial.begin(115200);
    init_uart();
    init_wifi();
    init_webserver();
    init_sd_logger();
    start_scan();
    send_uart("[+] Evil Twin v3 gestartet\n");
}

void loop() {
    handle_uart();
    scan_loop();
    run_deauth();
    hopper.hopChannel();
    delay(100);
}
