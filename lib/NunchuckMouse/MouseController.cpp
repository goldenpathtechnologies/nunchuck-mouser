//
// Created by Daryl G. Wright on 2022-12-29.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//

#include <Arduino.h>
#include "MouseController.h"

MouseController::MouseController(NunchuckController *device) {
    nunchuck = device;
}

void MouseController::handle() {
    auto xMovement = static_cast<int8_t>(
            nunchuck->getDirectionX() * getPrecision(nunchuck->getAnalogPercentX()));
    auto yMovement = static_cast<int8_t>(
            nunchuck->getDirectionY() * getPrecision(nunchuck->getAnalogPercentY()));

    if (nunchuck->isMoving()) {
        Mouse.move(xMovement, yMovement);
    }

    if (nunchuck->getCurrentInput().buttonZ) {
        Mouse.press();
    } else {
        Mouse.release();
    }

    if (nunchuck->getCurrentInput().buttonC) {
        Mouse.press(MOUSE_RIGHT);
    } else {
        Mouse.release(MOUSE_RIGHT);
    }
}

int MouseController::getPrecision(float analogPercentage) {
    float data = abs(analogPercentage);

    if (data < 30) {
        return 1;
    } else if (data < 40) {
        return 2;
    } else if (data < 50) {
        return 3;
    } else if (data < 60) {
        return 4;
    } else if (data < 70) {
        return 5;
    } else if (data < 80) {
        return 6;
    } else {
        return 7;
    }
}
