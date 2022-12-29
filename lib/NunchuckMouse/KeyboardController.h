//
// Created by Daryl G. Wright on 2022-12-29.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//

#ifndef NUNCHUCK_MOUSER_KEYBOARDCONTROLLER_H
#define NUNCHUCK_MOUSER_KEYBOARDCONTROLLER_H
#include "NunchuckController.h"

class KeyboardController {
public:
    explicit KeyboardController(NunchuckController *device);

    void handle();
    void updateKeyboardMode();
    void handleKeyboardTypingMode();
    void handleKeyboardNavigatingMode();
    void handleKeyboardQuickKeysMode();
    void handleKeyboardExitMode();
    void setActive(bool isActive);
    bool isActive() const;
private:
    enum KeyboardMode {
        TYPING,
        NAVIGATING,
        QUICK_KEYS,
        EXIT
    };
    NunchuckController *nunchuck;
    bool isActivated = false;
    KeyboardMode keyboardMode = TYPING;
};
#endif //NUNCHUCK_MOUSER_KEYBOARDCONTROLLER_H
