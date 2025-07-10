#ifndef CHANNEL_HOPPER_H
#define CHANNEL_HOPPER_H

#include <Arduino.h>
#include "comm_uart.h"

class ChannelHopper {
private:
    const int MIN_CHANNEL = 1;
    const int MAX_CHANNEL = 13;
    int current_channel = 1;
    unsigned long last_hop_time = 0;
    const unsigned long HOP_INTERVAL = 5000; // 5 Sekunden

public:
    void hopChannel();
    int getCurrentChannel();
};

#endif // CHANNEL_HOPPER_H
