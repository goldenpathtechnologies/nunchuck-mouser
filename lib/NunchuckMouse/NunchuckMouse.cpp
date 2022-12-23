//
// Created by Daryl G. Wright on 2022-12-22.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//
#include <NunchuckMouse.h>
#include <Arduino.h>

void NunchuckMouse::processInputs(NunchuckInput *input) {
    prevInput = currInput;
    currInput = *input;
}

NunchuckMouse::NunchuckMouse() = default;

void NunchuckMouse::begin() { // NOLINT(readability-convert-member-functions-to-static)
    Keyboard.begin();
    Mouse.begin();
}

float NunchuckMouse::getAnalogPercentX() const {
    if (currInput.analogX < LIVE_POS_X && currInput.analogX > LIVE_NEG_X) {
        return 0;
    } else if (currInput.analogX >= LIVE_POS_X) {
        return static_cast<float>((currInput.analogX - LIVE_POS_X) * 100) / static_cast<float>(RANGE_POS_X);
    } else {
        return static_cast<float>((LIVE_NEG_X - currInput.analogX) * -100) / static_cast<float>(RANGE_NEG_X);
    }
}

float NunchuckMouse::getAnalogPercentY() const {
    if (currInput.analogY < LIVE_POS_Y && currInput.analogY > LIVE_NEG_Y) {
        return 0;
    } else if (currInput.analogY >= LIVE_POS_Y) {
        return static_cast<float>((currInput.analogY - LIVE_POS_Y) * 100) / static_cast<float>(RANGE_POS_Y);
    } else {
        return static_cast<float>((LIVE_NEG_Y - currInput.analogY) * -100) / static_cast<float>(RANGE_NEG_Y);
    }
}

short NunchuckMouse::getDirectionX() const {
    if (prevInput.analogX == 0) return 0;

    if (currInput.analogX < LIVE_POS_X && currInput.analogX > LIVE_NEG_X) {
        return 0;
    } else if (currInput.analogX >= LIVE_POS_X) {
        return 1;
    } else {
        return -1;
    }
}

short NunchuckMouse::getDirectionY() const {
    if (prevInput.analogY == 0) return 0;

    if (currInput.analogY < LIVE_POS_Y && currInput.analogY > LIVE_NEG_Y) {
        return 0;
    } else if (currInput.analogX >= LIVE_POS_Y) {
        return 1;
    } else {
        return -1;
    }
}

bool NunchuckMouse::isMoving() const {
    return getDirectionX() != 0 || getDirectionY() != 0;
}
