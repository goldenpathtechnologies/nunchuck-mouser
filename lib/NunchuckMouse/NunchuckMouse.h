//
// Created by Daryl G. Wright on 2022-12-22.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//

#ifndef NUNCHUCK_MOUSER_NUNCHUCKMOUSE_H
#define NUNCHUCK_MOUSER_NUNCHUCKMOUSE_H
#include <Stream.h>
#include <Arduino.h>
#include <Constants.h>
#include "NunchuckController.h"
#include "MouseController.h"
#include "ScrollController.h"
#include "KeyboardController.h"

class NunchuckMouse {
public:
    NunchuckMouse();
    void processInputs(NunchuckInput *input);
    void begin();
    void printInputs(Stream &stream = Serial);

private:
    NunchuckController *nunchuck;
    MouseController *mouse;
    ScrollController *scroller;
    KeyboardController *keyboard;
    void updateMode();
};
#endif //NUNCHUCK_MOUSER_NUNCHUCKMOUSE_H
