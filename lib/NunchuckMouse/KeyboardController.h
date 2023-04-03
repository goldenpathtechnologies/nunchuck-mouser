//
// Created by Daryl G. Wright on 2022-12-29.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//

#ifndef NUNCHUCK_MOUSER_KEYBOARDCONTROLLER_H
#define NUNCHUCK_MOUSER_KEYBOARDCONTROLLER_H
#include <HID-Project.h>
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
    struct Keycode {
        KeyboardKeycode keyboardKey = KEY_RESERVED;
        ConsumerKeycode consumerKey = HID_CONSUMER_UNASSIGNED;
        explicit Keycode(KeyboardKeycode keyboardKey): keyboardKey(keyboardKey) {}
        explicit Keycode(ConsumerKeycode consumerKey): consumerKey(consumerKey) {}
    };
    NunchuckController *nunchuck;
    bool isActivated = false;
    KeyboardMode keyboardMode = NAVIGATION;
    void updateKeyboardMode();
    void handleNavigationMode();
    void handleNavigationPlusMode();
    void handleMediaMode();
    void handleExitMode();
    static void press(Keycode key);
    static void release(Keycode key);
    void handleDirectionalButtonPress(DirectionalButtons direction, Keycode key);
    void handleDirectionalButtonPress(DirectionalButtons direction, Keycode keys[], uint8_t numKeys);
    void handleCZButtonPress(NunchuckButtons button, Keycode key, bool noButtonRollover = false);
    void handleCZButtonPress(NunchuckButtons button, Keycode keys[], uint8_t numKeys, bool noButtonRollover = false);

};
#endif //NUNCHUCK_MOUSER_KEYBOARDCONTROLLER_H
