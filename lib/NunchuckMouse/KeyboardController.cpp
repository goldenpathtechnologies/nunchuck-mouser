//
// Created by Daryl G. Wright on 2022-12-29.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//

#include <Arduino.h>
#include "KeyboardController.h"

void KeyboardController::handle() {
    updateKeyboardMode();

    switch(keyboardMode) {
        case NAVIGATION_PLUS:
            handleNavigationPlusMode();
            break;
        case MEDIA:
            handleMediaMode();
            break;
        case EXIT:
            handleExitMode();
            break;
        default:
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
        keyboardMode = NAVIGATION_PLUS;
    } else if (nunchuck->isTiltedUp()) {
        keyboardMode = MEDIA;
    } else if (nunchuck->isTiltedDown()) {
        keyboardMode = EXIT;
    } else {
        keyboardMode = NAVIGATION;
    }
}

void KeyboardController::handleNavigationMode() {
    handleCZButtonPress(Z_BUTTON, KEY_ENTER, true);
    handleCZButtonPress(C_BUTTON, KEY_BACKSPACE, true);
    handleDirectionalButtonPress(UP, KEY_UP_ARROW);
    handleDirectionalButtonPress(DOWN, KEY_DOWN_ARROW);
    handleDirectionalButtonPress(LEFT, KEY_LEFT_ARROW);
    handleDirectionalButtonPress(RIGHT, KEY_RIGHT_ARROW);
}

void KeyboardController::handleNavigationPlusMode() {
    handleCZButtonPress(Z_BUTTON, KEY_SPACE, true);
    handleCZButtonPress(C_BUTTON, KEY_TAB, true);
    handleDirectionalButtonPress(LEFT, KEY_HOME);
    handleDirectionalButtonPress(RIGHT, KEY_END);
    handleDirectionalButtonPress(UP, KEY_PAGE_UP);
    handleDirectionalButtonPress(DOWN, KEY_PAGE_DOWN);
}

void KeyboardController::handleMediaMode() {
    // Note: Media keys will not work on Teensy 2.0 when Serial port is enabled;
    //  https://forum.pjrc.com/threads/34074-Keyboard-Media-Keys-now-(hopefully)-Windows-compatible?p=102516&viewfull=1#post102516
    //  Ensure build_flags field in platformio.ini is set to "-D USB_HID"
    handleCZButtonPress(Z_BUTTON, KEY_MEDIA_PLAY_PAUSE);
    handleCZButtonPress(C_BUTTON, KEY_MEDIA_MUTE);
    handleDirectionalButtonPress(UP, KEY_MEDIA_VOLUME_INC);
    handleDirectionalButtonPress(DOWN, KEY_MEDIA_VOLUME_DEC);
    handleDirectionalButtonPress(LEFT, KEY_MEDIA_PREV_TRACK);
    handleDirectionalButtonPress(RIGHT, KEY_MEDIA_NEXT_TRACK);
}

void KeyboardController::handleExitMode() {
    if (nunchuck->buttonCPressed() && !nunchuck->getCurrentInput().buttonZ) {
        setActive(false);
    }

    handleCZButtonPress(Z_BUTTON, KEY_ESC, true);
    handleDirectionalButtonPress(UP, MODIFIERKEY_GUI);
    handleDirectionalButtonPress(DOWN, KEY_DELETE);
    handleDirectionalButtonPress(
            LEFT,
            MODIFIERKEY_CTRL + MODIFIERKEY_ALT + MODIFIERKEY_SHIFT + MODIFIERKEY_GUI);
    handleDirectionalButtonPress(
            RIGHT,
            MODIFIERKEY_CTRL + MODIFIERKEY_ALT + MODIFIERKEY_SHIFT);
}

KeyboardController::KeyboardController(NunchuckController *device) {
    nunchuck = device;
}

void KeyboardController::handleDirectionalButtonPress(DirectionalButtons direction, int key) {
    if (nunchuck->directionPressed(direction)) {
        Keyboard.press(key);
    } else if (nunchuck->directionReleased(direction)) {
        Keyboard.release(key);
    }
}

void KeyboardController::handleCZButtonPress(NunchuckButtons button, int key, bool noButtonRollover) {
    if (button == C_BUTTON) {
        if (nunchuck->buttonCPressed() && !(nunchuck->getCurrentInput().buttonZ && noButtonRollover)) {
            Keyboard.press(key);
        } else if (nunchuck->buttonCReleased()) {
            Keyboard.release(key);
        }
    } else {
        if (nunchuck->buttonZPressed() && !(nunchuck->getCurrentInput().buttonC && noButtonRollover)) {
            Keyboard.press(key);
        } else if (nunchuck->buttonZReleased()) {
            Keyboard.release(key);
        }
    }
}
