//
// Created by Daryl G. Wright on 2023-01-01.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//

#include <Arduino.h>
#include "JoystickController.h"
#include "Constants.h"

JoystickController::JoystickController(NunchuckController *device) {
    nunchuck = device;

    resetAllInputs();
}

void JoystickController::handle() {
    if (nunchuck->isTiltedUp()) {
        handleSelectMode();
    } else if (nunchuck->isSideTilted()) {
        handleButtonMode();
    } else {
        switch(joystickMode) {
            case DIGITAL:
                handleDigitalMode();
                break;
            case FPS_LEFT:
                handleFPSLeftMode();
                break;
            case FPS_RIGHT:
                handleFPSRightMode();
                break;
            default:
                handleAnalogMode();
        }
    }
}

bool JoystickController::isActive() const {
    return isActivated;
}

void JoystickController::setActive(bool isActive) {
    isActivated = isActive;

    resetAllInputs();
}

void JoystickController::handleAnalogMode() {
    handleCZButtonPress(Z_BUTTON, B_BUTTON);
    handleCZButtonPress(C_BUTTON, A_BUTTON);

    Joystick.X(getAnalogValueX());
    Joystick.Y(getAnalogValueY());
}

void JoystickController::handleDigitalMode() {
    handleCZButtonPress(Z_BUTTON, B_BUTTON);
    handleCZButtonPress(C_BUTTON, A_BUTTON);

    handlePOVHat();
}

void JoystickController::handleFPSLeftMode() {
    handleCZButtonPress(Z_BUTTON, LT);
    handleCZButtonPress(C_BUTTON, LB);

    Joystick.X(getAnalogValueX());
    Joystick.Y(getAnalogValueY());
}

void JoystickController::handleFPSRightMode() {
    handleCZButtonPress(Z_BUTTON, RT);
    handleCZButtonPress(C_BUTTON, RB);

    Joystick.Z(getAnalogValueY());
    Joystick.Zrotate(getAnalogValueX());
}

void JoystickController::handleButtonMode() {
    handleCZButtonPress(Z_BUTTON, START);
    handleCZButtonPress(C_BUTTON, SELECT);
    handleDirectionalButtonPress(DOWN, B_BUTTON);
    handleDirectionalButtonPress(RIGHT, A_BUTTON);
    handleDirectionalButtonPress(LEFT, Y_BUTTON);
    handleDirectionalButtonPress(UP, X_BUTTON);
}

void JoystickController::handleSelectMode() {
    resetAllInputs();

    if (nunchuck->buttonCPressed()) {
        setActive(false);
    }

    if (nunchuck->directionPressed(UP)) {
        joystickMode = ANALOG;
    }

    if (nunchuck->directionPressed(DOWN)) {
        joystickMode = DIGITAL;
    }

    if (nunchuck->directionPressed(LEFT)) {
        joystickMode = FPS_LEFT;
    }

    if (nunchuck->directionPressed(RIGHT)) {
        joystickMode = FPS_RIGHT;
    }
}

void JoystickController::handleDirectionalButtonPress(DirectionalButtons direction, uint8_t joyButton) {
    handleButtonPress(
        nunchuck->directionPressed(direction),
        nunchuck->directionReleased(direction),
        joyButton);
}

void JoystickController::handleCZButtonPress(NunchuckButtons button, uint8_t joyButton) {
    if (button == C_BUTTON) {
        handleButtonPress(
            nunchuck->buttonCPressed(),
            nunchuck->buttonCReleased(),
            joyButton);
    } else {
        handleButtonPress(
            nunchuck->buttonZPressed(),
            nunchuck->buttonZReleased(),
            joyButton);
    }
}

void JoystickController::handleButtonPress(bool isPressed, bool isReleased, uint8_t joyButton) {
    if (isPressed) {
        Joystick.button(joyButton, true);
    } else if (isReleased) {
        Joystick.button(joyButton, false);
    }
}

void JoystickController::handlePOVHat() {
    switch (nunchuck->getDigitalDirection(50)) {
        case UP:
            Joystick.hat(0);
            break;
        case UP | RIGHT:
            Joystick.hat(45);
            break;
        case RIGHT:
            Joystick.hat(90);
            break;
        case DOWN | RIGHT:
            Joystick.hat(135);
            break;
        case DOWN:
            Joystick.hat(180);
            break;
        case DOWN | LEFT:
            Joystick.hat(225);
            break;
        case LEFT:
            Joystick.hat(270);
            break;
        case UP | LEFT:
            Joystick.hat(315);
            break;
        default:
            Joystick.hat(-1);
    }
}

uint16_t JoystickController::getAnalogValueX() {
    return getAnalogValue(nunchuck->getCurrentInput().analogX, CENTER_X);
}

uint16_t JoystickController::getAnalogValueY() {
    return getAnalogValue(nunchuck->getCurrentInput().analogY, CENTER_Y, true);
}

uint16_t JoystickController::getAnalogValue(int analog, int center, bool inverted) {
    int16_t joyAnalog;

    if (analog == center) {
        joyAnalog = 512;
    } else if (analog > center) {
        joyAnalog = 512 + ((analog - center) * (512 / 100));
    } else {
        joyAnalog = 512 - ((center - analog) * (512 / 100));
    }

    if (inverted) {
        joyAnalog = 1023 - joyAnalog;
    }

    if (joyAnalog < 0) {
        return 0;
    } else if (joyAnalog > 1023) {
        return 1023;
    } else {
        return static_cast<uint16_t>(joyAnalog);
    }
}

void JoystickController::resetAllInputs() {
    Joystick.hat(-1);
    Joystick.X(512);
    Joystick.Y(512);
    Joystick.Z(512);
    Joystick.Zrotate(512);
    for (int i = 1; i <= 10; i++) {
        Joystick.button(i, false);
    }
}
