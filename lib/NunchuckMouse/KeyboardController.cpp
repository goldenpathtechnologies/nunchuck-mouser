//
// Created by Daryl G. Wright on 2022-12-29.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//

#include <HID-Project.h>
#include "KeyboardController.h"

void KeyboardController::handle() {
    updateKeyboardMode();

    switch(keyboardMode) {
        case NAVIGATION_PLUS:
            statusLed->setStatusColor(0x00FFFF);
            handleNavigationPlusMode();
            break;
        case MEDIA:
            statusLed->setStatusColor(0xDAF7A6);
            handleMediaMode();
            break;
        case EXIT:
            statusLed->setStatusColor(0xFF00FF);
            handleExitMode();
            break;
        default:
            statusLed->setStatusColor(0xFFFF00);
            handleNavigationMode();
    }
}

void KeyboardController::setActive(bool isActive) {
    isActivated = isActive;
}

bool KeyboardController::isActive() const {
    return isActivated;
}

void KeyboardController::updateKeyboardMode() {
    if (nunchuck->isSideTilted()) {
        if (keyboardMode != NAVIGATION_PLUS) Keyboard.releaseAll();
        keyboardMode = NAVIGATION_PLUS;
    } else if (nunchuck->isTiltedUp()) {
        if (keyboardMode != MEDIA) Keyboard.releaseAll();
        keyboardMode = MEDIA;
    } else if (nunchuck->isTiltedDown()) {
        if (keyboardMode != EXIT) Keyboard.releaseAll();
        keyboardMode = EXIT;
    } else {
        if (keyboardMode != NAVIGATION) Keyboard.releaseAll();
        keyboardMode = NAVIGATION;
    }
}

void KeyboardController::handleNavigationMode() {
    handleCZButtonPress(Z_BUTTON, Keycode(KEY_ENTER), true);
    handleCZButtonPress(C_BUTTON, Keycode(KEY_BACKSPACE), true);
    handleDirectionalButtonPress(UP, Keycode(KEY_UP_ARROW));
    handleDirectionalButtonPress(DOWN, Keycode(KEY_DOWN_ARROW));
    handleDirectionalButtonPress(LEFT, Keycode(KEY_LEFT_ARROW));
    handleDirectionalButtonPress(RIGHT, Keycode(KEY_RIGHT_ARROW));
}

void KeyboardController::handleNavigationPlusMode() {
    handleCZButtonPress(Z_BUTTON, Keycode(KEY_SPACE), true);
    handleCZButtonPress(C_BUTTON, Keycode(KEY_TAB), true);
    handleDirectionalButtonPress(LEFT, Keycode(KEY_HOME));
    handleDirectionalButtonPress(RIGHT, Keycode(KEY_END));
    handleDirectionalButtonPress(UP, Keycode(KEY_PAGE_UP));
    handleDirectionalButtonPress(DOWN, Keycode(KEY_PAGE_DOWN));
}

void KeyboardController::handleMediaMode() {
    handleCZButtonPress(Z_BUTTON, Keycode(MEDIA_PLAY_PAUSE));
    handleCZButtonPress(C_BUTTON, Keycode(MEDIA_VOLUME_MUTE));
    handleDirectionalButtonPress(UP, Keycode(MEDIA_VOLUME_UP));
    handleDirectionalButtonPress(DOWN, Keycode(MEDIA_VOLUME_DOWN));
    handleDirectionalButtonPress(LEFT, Keycode(MEDIA_PREV));
    handleDirectionalButtonPress(RIGHT, Keycode(MEDIA_NEXT));
}

void KeyboardController::handleExitMode() {
    if (nunchuck->buttonCPressed() && !nunchuck->getCurrentInput().buttonZ) {
        setActive(false);
    }

    handleCZButtonPress(Z_BUTTON, Keycode(KEY_ESC), true);
    handleDirectionalButtonPress(UP, Keycode(KEY_LEFT_GUI));
    handleDirectionalButtonPress(DOWN, Keycode(KEY_DELETE));

    Keycode hyperKeys[4] = {Keycode(KEY_LEFT_CTRL), Keycode(KEY_LEFT_ALT), Keycode(KEY_LEFT_SHIFT), Keycode(KEY_LEFT_GUI)};
    Keycode mehKeys[3] = {Keycode(KEY_LEFT_CTRL), Keycode(KEY_LEFT_ALT), Keycode(KEY_LEFT_SHIFT)};
    handleDirectionalButtonPress(LEFT, hyperKeys, 4);
    handleDirectionalButtonPress(RIGHT, mehKeys, 3);
}

KeyboardController::KeyboardController(NunchuckController *device, LedController *led) {
    nunchuck = device;
    statusLed = led;
}

void KeyboardController::press(Keycode key) {
    if (key.keyboardKey != KEY_RESERVED) {
        Keyboard.press(key.keyboardKey);
    } else if (key.consumerKey != HID_CONSUMER_UNASSIGNED) {
        Consumer.press(key.consumerKey);
    }
}

void KeyboardController::release(Keycode key) {
    if (key.keyboardKey != KEY_RESERVED) {
        Keyboard.release(key.keyboardKey);
    } else if (key.consumerKey != HID_CONSUMER_UNASSIGNED) {
        Consumer.release(key.consumerKey);
    }
}

void KeyboardController::handleDirectionalButtonPress(DirectionalButtons direction, Keycode key) {
    if (nunchuck->directionPressed(direction)) {
        press(key);
    } else if (nunchuck->directionReleased(direction)) {
        release(key);
    }
}

void KeyboardController::handleDirectionalButtonPress(DirectionalButtons direction, Keycode keys[], uint8_t numKeys) {
    for (int i = 0; i < numKeys; i++) {
        handleDirectionalButtonPress(direction, keys[i]);
    }
}

void KeyboardController::handleCZButtonPress(NunchuckButtons button, Keycode key, bool noButtonRollover) {
    if (button == C_BUTTON) {
        if (nunchuck->buttonCPressed() && !(nunchuck->getCurrentInput().buttonZ && noButtonRollover)) {
            press(key);
        } else if (nunchuck->buttonCReleased()) {
            release(key);
        }
    } else {
        if (nunchuck->buttonZPressed() && !(nunchuck->getCurrentInput().buttonC && noButtonRollover)) {
            press(key);
        } else if (nunchuck->buttonZReleased()) {
            release(key);
        }
    }
}

void KeyboardController::handleCZButtonPress(NunchuckButtons button, Keycode keys[], uint8_t numKeys, bool noButtonRollover) {
    for (int i = 0; i < numKeys; i++) {
        handleCZButtonPress(button, keys[i], noButtonRollover);
    }
}
