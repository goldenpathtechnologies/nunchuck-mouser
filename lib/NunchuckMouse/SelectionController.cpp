//
// Created by Daryl G. Wright on 2022-12-29.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//

#include <Arduino.h>
#include "SelectionController.h"
#include "JoystickController.h"
#include "KeyboardController.h"

SelectionController::SelectionController(
        NunchuckController *device,
        JoystickController *joystickController,
        KeyboardController *keyboardController) {
    nunchuck = device;
    joystick = joystickController;
    keyboard = keyboardController;
}

void SelectionController::handle() {
    if (nunchuck->directionPressed(DOWN)) {
        joystick->setActive(true);
    } else if (nunchuck->directionPressed(UP)) {
        keyboard->setActive(true);
    }
}

bool SelectionController::isActive() {
    return nunchuck->pitchAngleInRange(60, 115)
    && nunchuck->rollAngleInRange(-60, 60)
    && nunchuck->getMode() != JOYSTICK
    && nunchuck->getMode() != KEYBOARD;
}
