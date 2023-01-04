//
// Created by Daryl G. Wright on 2023-01-01.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//

#ifndef NUNCHUCK_MOUSER_JOYSTICKCONTROLLER_H
#define NUNCHUCK_MOUSER_JOYSTICKCONTROLLER_H
#include "NunchuckController.h"

class JoystickController {
public:
    explicit JoystickController(NunchuckController *device);

    void handle();
    bool isActive() const;
    void setActive(bool isActive);
private:
    enum JoystickMode {
        ANALOG,
        DIGITAL,
        FPS_LEFT,
        FPS_RIGHT
    };
    enum JoystickButtons {
        B_BUTTON = 1,
        A_BUTTON,
        Y_BUTTON,
        X_BUTTON,
        LT,
        RT,
        LB,
        RB,
        SELECT,
        START
    };
    NunchuckController *nunchuck;
    bool isActivated = false;
    JoystickMode joystickMode = ANALOG;
    void handleAnalogMode();
    void handleDigitalMode();
    void handleFPSLeftMode();
    void handleFPSRightMode();
    void handleButtonMode();
    void handleSelectMode();
    void handleDirectionalButtonPress(DirectionalButtons direction, uint8_t joyButton);
    void handleCZButtonPress(NunchuckButtons button, uint8_t joyButton);
    static void handleButtonPress(bool isPressed, bool isReleased, uint8_t joyButton);
    uint16_t getAnalogValueX();
    uint16_t getAnalogValueY();
    static uint16_t getAnalogValue(int analog, int center, bool inverted = false);
    void handlePOVHat();
    static void resetAllInputs();
};
#endif //NUNCHUCK_MOUSER_JOYSTICKCONTROLLER_H
