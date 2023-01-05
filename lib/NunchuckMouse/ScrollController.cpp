//
// Created by Daryl G. Wright on 2022-12-29.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//

#include "ScrollController.h"

ScrollController::ScrollController(NunchuckController *device) {
    nunchuck = device;
}

bool ScrollController::isActive() {
    return nunchuck->isSideTilted()
    && (nunchuck->getMode() == MOUSE || nunchuck->getMode() == SCROLL);
}

void ScrollController::handle() {
    auto xMovement =
            static_cast<int8_t>(
                    nunchuck->getDirectionX()
                    * getPrecision(nunchuck->getAnalogPercentX()));
    auto yMovement =
            static_cast<int8_t>(
                    nunchuck->getDirectionY()
                    * getPrecision(nunchuck->getAnalogPercentY()));
    int yScrollDelay = 100;
    int xScrollDelay = 50;

    if (abs(yMovement) < 3) {
        scrollMaxTimerY = 0;
    } else if (scrollMaxTimerY > 2000 && scrollMaxTimerY <= 4500) {
        yScrollDelay = 50;
    } else if (scrollMaxTimerY > 4500) {
        yScrollDelay = 0;
    }

    if (abs(xMovement) < 3) {
        scrollMaxTimerX = 0;
    } else if (scrollMaxTimerX > 2000) {
        xScrollDelay = 50;
    }

    if (yMovement != 0 && scrollDelayTimer > yScrollDelay) {
        Mouse.scroll(yMovement);
        scrollDelayTimer = 0;
    } else if (xMovement != 0 && scrollDelayTimer > xScrollDelay) {
        Mouse.scroll(0, xMovement);
        scrollDelayTimer = 0;
    }

    if (nunchuck->buttonCPressed()) {
        Mouse.press(MOUSE_MIDDLE);
    } else if (nunchuck->buttonCReleased()) {
        Mouse.release(MOUSE_MIDDLE);
    }
}

int ScrollController::getPrecision(float analogPercentage) {
    float data = abs(analogPercentage);

    if (data < 50) {
        return 1;
    } else if (data < 70) {
        return 2;
    } else {
        return 3;
    }
}
