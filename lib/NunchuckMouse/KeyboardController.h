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
    void setActive(bool isActive);
    bool isActive() const;
private:
    enum KeyboardMode {
        NAVIGATION,
        NAVIGATION_PLUS,
        MEDIA,
        EXIT
    };
    NunchuckController *nunchuck;
    bool isActivated = false;
    KeyboardMode keyboardMode = NAVIGATION;
    void updateKeyboardMode();
    void handleNavigationMode();
    void handleNavigationPlusMode();
    void handleMediaMode();
    void handleExitMode();
    void handleDirectionalButtonPress(DirectionalButtons direction, int key);
    void handleCZButtonPress(NunchuckButtons button, int key, bool noButtonRollover = false);
};
#endif //NUNCHUCK_MOUSER_KEYBOARDCONTROLLER_H
