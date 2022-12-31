//
// Created by Daryl G. Wright on 2022-12-29.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//

#include "SelectionController.h"
#include "FreehandController.h"
#include "KeyboardController.h"

SelectionController::SelectionController(
        NunchuckController *device,
        FreehandController *freehandController,
        KeyboardController *keyboardController) {
    nunchuck = device;
    freehandMouse = freehandController;
    keyboard = keyboardController;
}

void SelectionController::handle() {
    if (nunchuck->directionPressed(UP)) {
        freehandMouse->setActive(true);
    } else if (nunchuck->directionPressed(DOWN)) {
        keyboard->setActive(true);
    }
}

bool SelectionController::isActive() {
    return nunchuck->pitchAngleInRange(60, 115)
    && nunchuck->rollAngleInRange(-60, 60)
    && nunchuck->getMode() != FREEHAND
    && nunchuck->getMode() != KEYBOARD;
}
