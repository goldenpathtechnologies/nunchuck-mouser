//
// Created by Daryl G. Wright on 2022-12-29.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//

#include "Constants.h"
#include "NunchuckController.h"

int NunchuckController::getDirectionX() const {
    if (prevInput.analogX == 0) return 0;

    if (currInput.analogX < LIVE_POS_X && currInput.analogX > LIVE_NEG_X) {
        return 0;
    } else if (currInput.analogX >= LIVE_POS_X) {
        return 1;
    } else {
        return -1;
    }
}

int NunchuckController::getDirectionY() const {
    if (prevInput.analogY == 0) return 0;

    if (currInput.analogY < LIVE_POS_Y && currInput.analogY > LIVE_NEG_Y) {
        return 0;
    } else if (currInput.analogY >= LIVE_POS_Y) {
        return -1;
    } else {
        return 1;
    }
}

bool NunchuckController::isMoving() const {
    return getDirectionX() != 0 || getDirectionY() != 0;
}

float NunchuckController::getAnalogPercentX() const {
    if (currInput.analogX < LIVE_POS_X && currInput.analogX > LIVE_NEG_X) {
        return 0;
    } else if (currInput.analogX >= LIVE_POS_X) {
        return static_cast<float>(
                (currInput.analogX - LIVE_POS_X) * 100) / static_cast<float>(RANGE_POS_X);
    } else {
        return static_cast<float>(
                (LIVE_NEG_X - currInput.analogX) * -100) / static_cast<float>(RANGE_NEG_X);
    }
}

float NunchuckController::getAnalogPercentY() const {
    if (currInput.analogY < LIVE_POS_Y && currInput.analogY > LIVE_NEG_Y) {
        return 0;
    } else if (currInput.analogY >= LIVE_POS_Y) {
        return static_cast<float>(
                (currInput.analogY - LIVE_POS_Y) * 100) / static_cast<float>(RANGE_POS_Y);
    } else {
        return static_cast<float>(
                (LIVE_NEG_Y - currInput.analogY) * -100) / static_cast<float>(RANGE_NEG_Y);
    }
}

bool NunchuckController::angleInRange(float value, float a0, float a1) {
    if (a0 == a1) return false;

    if (a0 < a1) {
        return value > a0 && value < a1;
    } else {
        return (value > a0 && value < 180) || (value > -180 && value < a1);
    }
}

bool NunchuckController::rollAngleInRange(float a0, float a1) const {
    return angleInRange(currInput.rollAngle, a0, a1);
}

bool NunchuckController::pitchAngleInRange(float a0, float a1) const {
    return angleInRange(currInput.pitchAngle, a0, a1);
}

bool NunchuckController::isSideTilted() const {
    return (rollAngleInRange(40, 120) || rollAngleInRange(-120, -40))
    && pitchAngleInRange(-35, 35);
}

bool NunchuckController::isTiltedUp() const {
    return pitchAngleInRange(35, 90);
}

bool NunchuckController::isTiltedDown() const {
    return pitchAngleInRange(-90, -35);
}

void NunchuckController::updateDirectionalState() {
    prevDirectionalState = currDirectionalState;

    currDirectionalState = getDigitalDirection();
}

int NunchuckController::getDigitalDirection() const {
    int direction = NONE;

    if (getAnalogPercentX() > 95) {
        direction |= RIGHT;
    } else if (getAnalogPercentX() < -95) {
        direction |= LEFT;
    }

    if (getAnalogPercentY() > 95) {
        direction |= UP;
    } else if (getAnalogPercentY() < -95) {
        direction |= DOWN;
    }

    return direction;
}

bool NunchuckController::isActiveDirection(int direction) const {
    return (getDigitalDirection() & direction) == direction;
}

NunchuckInput NunchuckController::getCurrentInput() {
    return currInput;
}

NunchuckInput NunchuckController::getPreviousInput() {
    return prevInput;
}

void NunchuckController::updateInput(NunchuckInput *input) {
    prevInput = currInput;
    currInput = *input;
}

NunchuckMode NunchuckController::getMode() {
    return mode;
}

void NunchuckController::setMode(NunchuckMode m) {
    mode = m;
}
