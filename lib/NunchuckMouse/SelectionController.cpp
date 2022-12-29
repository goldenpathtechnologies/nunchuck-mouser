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
    // TODO: Use digital method to get direction here.
    int yDirection = nunchuck->getDirectionY();

    if (yDirection == -1) {
        freehandMouse->setActive(true);
    } else if (yDirection == 1) {
        keyboard->setActive(true);
    }
}

bool SelectionController::isActive() {
    return nunchuck->pitchAngleInRange(60, 115) && nunchuck->getMode() != FREEHAND;
}
