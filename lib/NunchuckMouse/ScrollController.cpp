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

    if (yMovement != 0 && scrollDelay > 100) {
        Mouse.scroll(yMovement);
        scrollDelay = 0;
    } else if (xMovement != 0 && scrollDelay > 50) {
        Mouse.scroll(0, xMovement);
        scrollDelay = 0;
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

    // TODO: If scrolling occurs for more than a few seconds, increase the precision by a lot.
}
