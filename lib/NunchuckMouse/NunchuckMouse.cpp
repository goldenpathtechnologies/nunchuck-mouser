//
// Created by Daryl G. Wright on 2022-12-22.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//
#include <NunchuckMouse.h>
#include <Arduino.h>

void NunchuckMouse::processInputs(NunchuckInput *input) {
    prevInput = currInput;
    currInput = *input;

    // TODO: If the prev and curr input is the same for a certain amount of time, put the Keyboard and Mouse to sleep.
    //  When this changes, wake them up.

    updateMode();

    switch (mode) {
        case SCROLL:
            handleScrollMode();
            break;
        default:
            handleMouseMode();
    }
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

int NunchuckMouse::getDirectionX() const {
    if (prevInput.analogX == 0) return 0;

    if (currInput.analogX < LIVE_POS_X && currInput.analogX > LIVE_NEG_X) {
        return 0;
    } else if (currInput.analogX >= LIVE_POS_X) {
        return 1;
    } else {
        return -1;
    }
}

int NunchuckMouse::getDirectionY() const {
    if (prevInput.analogY == 0) return 0;

    if (currInput.analogY < LIVE_POS_Y && currInput.analogY > LIVE_NEG_Y) {
        return 0;
    } else if (currInput.analogY >= LIVE_POS_Y) {
        return -1;
    } else {
        return 1;
    }
}

bool NunchuckMouse::isMoving() const {
    return getDirectionX() != 0 || getDirectionY() != 0;
}

void NunchuckMouse::printInputs(Stream &stream) {
    stream.print("X: ");
    stream.print(getAnalogPercentX());
    stream.print(" | Y: ");
    stream.print(getAnalogPercentY());
    stream.print(" | Roll angle: ");
    stream.print(currInput.rollAngle);
    stream.print(" | Pitch angle: ");
    stream.print(currInput.pitchAngle);
    stream.print(" | Mode: ");
    switch (mode) {
        case SCROLL:
            stream.print("SCROLL");
            break;
        default:
            stream.print("MOUSE");
    }
    stream.println();
}

void NunchuckMouse::updateMode() {
    if (scrollModeActivated()) {
        mode = SCROLL;
    } else {
        mode = MOUSE;
    }
}

bool NunchuckMouse::rollAngleInRange(float a0, float a1) const {
    return angleInRange(currInput.rollAngle, a0, a1);
}

bool NunchuckMouse::pitchAngleInRange(float a0, float a1) const {
    return angleInRange(currInput.pitchAngle, a0, a1);
}

bool NunchuckMouse::angleInRange(float value, float a0, float a1) {
    if (a0 == a1) return false;

    if (a0 < a1) {
        return value > a0 && value < a1;
    } else {
        return (value > a0 && value < 180) || (value > -180 && value < a1);
    }
}

bool NunchuckMouse::scrollModeActivated() {
    return (rollAngleInRange(40, 120) || rollAngleInRange(-120, -40))
        && pitchAngleInRange(-35, 35);
}

void NunchuckMouse::handleMouseMode() {
    auto xMovement = static_cast<int8_t>(getDirectionX() * getPrecision(getAnalogPercentX()));
    auto yMovement = static_cast<int8_t>(getDirectionY() * getPrecision(getAnalogPercentY()));

    if (isMoving()) {
        Mouse.move(xMovement, yMovement);
    }

    if (currInput.buttonZ) {
        Mouse.press();
    } else {
        Mouse.release();
    }

    if (currInput.buttonC) {
        Mouse.press(MOUSE_RIGHT);
    } else {
        Mouse.release(MOUSE_RIGHT);
    }

    Serial.printf("X: %2d, Y: %2d\n", xMovement, yMovement);
}

int NunchuckMouse::getPrecision(float analogPercentage) {
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

void NunchuckMouse::handleScrollMode() {
    auto xMovement = static_cast<int8_t>(getDirectionX() * getScrollPrecision(getAnalogPercentX()));
    auto yMovement = static_cast<int8_t>(getDirectionY() * getScrollPrecision(getAnalogPercentY()));

    if (yMovement != 0 && scrollDelay > 100) {
        Mouse.scroll(yMovement);
        scrollDelay = 0;
    } else if (xMovement != 0 && scrollDelay > 50) {
        Mouse.scroll(0, xMovement);
        scrollDelay = 0;
    }
}

int NunchuckMouse::getScrollPrecision(float analogPercentage) {
    float data = abs(analogPercentage);

    if (data < 50) {
        return 1;
    } else if (data < 70) {
        return 2;
    } else {
        return 3;
    }
}
