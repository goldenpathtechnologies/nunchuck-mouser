//
// Created by Daryl G. Wright on 2022-12-29.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//

#include "FreehandController.h"

FreehandController::FreehandController(NunchuckController *device) {
    nunchuck = device;
}

void FreehandController::handle() {
//    auto xMovement = static_cast<int8_t>(getPrecision(currInput.rollAngle));
//    auto yMovement = static_cast<int8_t>(-1 * getPrecision(currInput.pitchAngle));

    if (nunchuck->buttonCPressed()) {
        setActive(false);
    }
    // TODO: Disabling movement in freehand mode until the algorithm is refined. Using roll and pitch angle is unwieldy.
    //  I'll have to use the accelX, accelY, and accelZ values directly in an equation.
    //Mouse.move(xMovement, yMovement);
}

int FreehandController::getPrecision(float analogPercentage) {
//    float data = abs(analogPercentage);
//    int precision = 0;
//    int direction = static_cast<int>(analogPercentage / data);
//
//    // Rolling dead zone -15 to 15
//    // Max roll +/- 55
//    // Pitch dead zone -20 to 20
//    // Max pitch +/- 60
//    // Greater than max has no movement, which allows for selection mode with given numbers
//
//    if (data < 15) {
//        precision = 0;
//    } else if (data < 25) {
//        precision = 1;
//    } else if (data < 30) {
//        precision = 2;
//    } else if (data < 35) {
//        precision = 3;
//    } else if (data < 40) {
//        precision = 4;
//    } else if (data < 45) {
//        precision = 5;
//    } else if (data < 50) {
//        precision = 6;
//    } else if (data < 55) {
//        precision = 7;
//    }
//
//    return precision * direction;
    return 0;
}

bool FreehandController::isActive() const {
    return isActivated;
}

void FreehandController::setActive(bool isActive) {
    isActivated = isActive;
}
