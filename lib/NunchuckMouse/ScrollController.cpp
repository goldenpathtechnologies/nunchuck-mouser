//
// Created by Daryl G. Wright on 2022-12-29.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//

#include "ScrollController.h"
#include "Constants.h"

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
                    * getScrollSpeed(nunchuck->getAnalogPercentX()));
    auto yMovement =
            static_cast<int8_t>(
                    nunchuck->getDirectionY()
                    * getScrollSpeed(nunchuck->getAnalogPercentY()));
    int yScrollDelay = SCROLL_DELAY_Y;
    int xScrollDelay = SCROLL_DELAY_X;

    if (abs(yMovement) < FAST) {
        scrollMaxTimerY = 0;
    } else if (scrollMaxTimerY > FAST_SCROLL_ACTIVATION_TIME
            && scrollMaxTimerY <= MAX_SCROLL_ACTIVATION_TIME) {
        yScrollDelay = SCROLL_DELAY_Y_FAST;
    } else if (scrollMaxTimerY > MAX_SCROLL_ACTIVATION_TIME) {
        yScrollDelay = SCROLL_DELAY_Y_MAX;
    }

    if (abs(xMovement) < FAST) {
        scrollMaxTimerX = 0;
    } else if (scrollMaxTimerX > FAST_SCROLL_ACTIVATION_TIME) {
        xScrollDelay = SCROLL_DELAY_X_FAST;
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

int ScrollController::getScrollSpeed(float analogPercentage) {
    float data = abs(analogPercentage);

    if (data < 50) {
        return SLOW;
    } else if (data < 70) {
        return MEDIUM;
    } else {
        return FAST;
    }
}
