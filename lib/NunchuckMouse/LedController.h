//
// Created by Daryl G. Wright on 2023-04-06.
// Copyright (c) 2023 Golden Path Technologies Inc. MIT Licence.
//

#ifndef NUNCHUCK_MOUSER_LEDCONTROLLER_H
#define NUNCHUCK_MOUSER_LEDCONTROLLER_H

#include <elapsedMillis.h>
#include "Adafruit_DotStar.h"

class LedController {
public:
    LedController();
    void update();
    void setStatusColor(uint32_t color);
    void resetStatusColor();
    void enableActivityIndicator();
    void disableActivityIndicator();
private:
    const uint16_t NUM_LEDS = 1;
    const uint8_t DATA_PIN = 7;
    const uint8_t CLOCK_PIN = 8;
    const uint32_t defaultColor = 0x008000;
    Adafruit_DotStar *statusLed;
    bool activityLedOn;
    elapsedMillis delayTimer;
    uint32_t currentColor;
};
#endif //NUNCHUCK_MOUSER_LEDCONTROLLER_H
