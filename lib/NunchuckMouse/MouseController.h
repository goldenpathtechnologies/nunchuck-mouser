//
// Created by Daryl G. Wright on 2022-12-29.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//

#ifndef NUNCHUCK_MOUSER_MOUSECONTROLLER_H
#define NUNCHUCK_MOUSER_MOUSECONTROLLER_H
#include "NunchuckController.h"
#include "KeyboardController.h"

class MouseController {
public:
    explicit MouseController(
            NunchuckController *device,
            KeyboardController *keyboardController);

    void handle();
    static int getPrecision(float analogPercentage);
private:
    NunchuckController *nunchuck;
    KeyboardController *keyboard;
    bool handleSwitchToKeyboardMode();
};
#endif //NUNCHUCK_MOUSER_MOUSECONTROLLER_H
