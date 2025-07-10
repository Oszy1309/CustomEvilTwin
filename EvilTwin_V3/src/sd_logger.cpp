#include <SPI.h>
#include <SD.h>
#include "config.h"

File logFile;

void init_sd_logger() {
    if (!SD.begin(SD_CS_PIN)) {
        Serial.println("SD-Initialisierung fehlgeschlagen!");
        return;
    }

    logFile = SD.open("/pmkid_log.txt", FILE_APPEND);
    if (!logFile) {
        Serial.println("Fehler beim Öffnen der Logdatei!");
    }
}

void save_pmkid_to_sd(const void* pkt_data) {
    if (!logFile) return;

    logFile.write((const uint8_t*)pkt_data, 256); // Beispiel: nur ersten 256 Bytes
    logFile.flush();
}
