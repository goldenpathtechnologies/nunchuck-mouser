//
// Created by Daryl G. Wright on 2023-01-01.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//

#include <Arduino.h>
#include "JoystickController.h"
#include "Constants.h"

JoystickController::JoystickController(NunchuckController *device) {
    nunchuck = device;
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
    // TODO: Reset all joystick buttons to default values to prevent buttons from sticking when
    //  exiting joystick mode.
    isActivated = isActive;
}

void JoystickController::handleAnalogMode() {
    handleCZButtonPress(Z_BUTTON, B_BUTTON);
    handleCZButtonPress(C_BUTTON, A_BUTTON);

    // TODO: Analog is not working at all as expected, mainly sticking to one corner and moving
    //  randomly.
    Joystick.X(getAnalogValueX());
    Joystick.Y(getAnalogValueY());
}

void JoystickController::handleDigitalMode() {
    handleCZButtonPress(Z_BUTTON, B_BUTTON);
    handleCZButtonPress(C_BUTTON, A_BUTTON);

    // TODO: POV hat does not detect diagonal directions. Also, the hat direction should actuate at
    //  50% of the analog movement instead of 95%.
    handlePOVHat();
}

void JoystickController::handleFPSLeftMode() {
    handleCZButtonPress(Z_BUTTON, LT);
    handleCZButtonPress(C_BUTTON, LB);

    // TODO: Same analog issue as in Analog Mode since it is the same method.
    Joystick.X(getAnalogValueX());
    Joystick.Y(getAnalogValueX());
}

void JoystickController::handleFPSRightMode() {
    handleCZButtonPress(Z_BUTTON, RT);
    handleCZButtonPress(C_BUTTON, RB);

    // TODO: Z axes appear to move normally for the first few analog steps but quickly maxes out.
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
    switch (nunchuck->getDigitalDirection()) {
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
    return nunchuck->getCurrentInput().analogX * 1024 / (MAX_X - MIN_X);
}

uint16_t JoystickController::getAnalogValueY() {
    return nunchuck->getCurrentInput().analogY * 1024 / (MAX_Y - MIN_Y);
}
