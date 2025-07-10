#include <ESPAsyncWebServer.h>
#include "config.h"
#include "target.h"
#include "wifi_manager.h"
#include "deauth.h"

AsyncWebServer server(80);

void init_webserver() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(200, "text/plain", "Evil Twin v3 Interface");
    });

    server.on("/scan", HTTP_GET, [](AsyncWebServerRequest* request) {
        start_scan();
        request->send(200, "text/plain", "Scan gestartet");
    });

    server.on("/deauth/start", HTTP_GET, [](AsyncWebServerRequest* request) {
        start_deauth();
        request->send(200, "text/plain", "Deauth gestartet");
    });

    server.on("/deauth/stop", HTTP_GET, [](AsyncWebServerRequest* request) {
        stop_deauth();
        request->send(200, "text/plain", "Deauth gestoppt");
    });

    server.begin();
}
