//
// Created by Daryl G. Wright on 2022-12-29.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//

#ifndef NUNCHUCK_MOUSER_SCROLLCONTROLLER_H
#define NUNCHUCK_MOUSER_SCROLLCONTROLLER_H
#include <Arduino.h>
#include "NunchuckController.h"

class ScrollController {
public:
    explicit ScrollController(NunchuckController *device);

    bool isActive();
    void handle();
private:
    enum ScrollSpeed {
        SLOW = 1,
        MEDIUM,
        FAST
    };
    NunchuckController *nunchuck;
    elapsedMillis scrollDelayTimer;
    elapsedMillis scrollMaxTimerY;
    elapsedMillis scrollMaxTimerX;
    static int getScrollSpeed(float analogPercentage);
};
#endif //NUNCHUCK_MOUSER_SCROLLCONTROLLER_H
