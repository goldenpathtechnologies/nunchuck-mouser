//
// Created by Daryl G. Wright on 2022-12-29.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//

#ifndef NUNCHUCK_MOUSER_SELECTIONCONTROLLER_H
#define NUNCHUCK_MOUSER_SELECTIONCONTROLLER_H
#include "NunchuckController.h"
#include "FreehandController.h"
#include "KeyboardController.h"

class SelectionController {
public:
    explicit SelectionController(
            NunchuckController *device,
            FreehandController *freehandController,
            KeyboardController *keyboardController);

    void handle();
    bool isActive();
private:
    NunchuckController *nunchuck;
    FreehandController *freehandMouse;
    KeyboardController *keyboard;
};
#endif //NUNCHUCK_MOUSER_SELECTIONCONTROLLER_H
