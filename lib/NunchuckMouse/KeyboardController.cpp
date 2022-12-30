//
// Created by Daryl G. Wright on 2022-12-29.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//

#include <Arduino.h>
#include "KeyboardController.h"

void KeyboardController::handle() {
    updateKeyboardMode();

    switch(keyboardMode) {
        case NAVIGATING:
            handleKeyboardNavigatingMode();
            break;
        case QUICK_KEYS:
            handleKeyboardQuickKeysMode();
            break;
        case EXIT:
            handleKeyboardExitMode();
            break;
        default:
            handleKeyboardTypingMode();
    }
}

void KeyboardController::updateKeyboardMode() {
    if (nunchuck->isSideTilted()) {
        keyboardMode = NAVIGATING;
    } else if (nunchuck->isTiltedUp()) {
        keyboardMode = QUICK_KEYS;
    } else if (nunchuck->isTiltedDown()) {
        keyboardMode = EXIT;
    } else {
        keyboardMode = TYPING;
    }
}

void KeyboardController::handleKeyboardTypingMode() {

}

void KeyboardController::handleKeyboardNavigatingMode() {
    if (nunchuck->directionPressed(RIGHT)) {
        Keyboard.press(KEY_RIGHT_ARROW);
    } else if (nunchuck->directionReleased(RIGHT)) {
        Keyboard.release(KEY_RIGHT_ARROW);
    }

    if (nunchuck->directionPressed(LEFT)) {
        Keyboard.press(KEY_LEFT_ARROW);
    } else if (nunchuck->directionReleased(LEFT)) {
        Keyboard.release(KEY_LEFT_ARROW);
    }

    if (nunchuck->directionPressed(UP)) {
        Keyboard.press(KEY_UP_ARROW);
    } else if (nunchuck->directionReleased(UP)) {
        Keyboard.release(KEY_UP_ARROW);
    }

    if (nunchuck->directionPressed(DOWN)) {
        Keyboard.press(KEY_DOWN_ARROW);
    } else if (nunchuck->directionReleased(DOWN)) {
        Keyboard.release(KEY_DOWN_ARROW);
    }

    if (nunchuck->buttonZPressed() && !nunchuck->getCurrentInput().buttonC) {
        Keyboard.press(KEY_ENTER);
    } else if (nunchuck->buttonZReleased()) {
        Keyboard.release(KEY_ENTER);
    }

    if (nunchuck->buttonCPressed() && !nunchuck->getCurrentInput().buttonZ) {
        Keyboard.press(KEY_BACKSPACE);
    } else if (nunchuck->buttonCReleased()) {
        Keyboard.release(KEY_BACKSPACE);
    }
}

void KeyboardController::handleKeyboardQuickKeysMode() {
    if (nunchuck->buttonZPressed()) {
        Keyboard.press(KEY_SPACE);
    } else if (nunchuck->buttonZReleased()) {
        Keyboard.release(KEY_SPACE);
    }

    if (nunchuck->getCurrentInput().buttonC) {
        if (nunchuck->directionPressed(UP)) {
            Keyboard.press(KEY_MINUS);
        } else if (nunchuck->directionReleased(UP)) {
            Keyboard.release(KEY_MINUS);
        }

        if (nunchuck->directionPressed(DOWN)) {
            // Question mark `?`
            Keyboard.press(MODIFIERKEY_LEFT_SHIFT);
            Keyboard.press(KEY_SLASH);
        } else if (nunchuck->directionReleased(DOWN)) {
            Keyboard.release(KEY_SLASH);
            Keyboard.release(MODIFIERKEY_LEFT_SHIFT);
        }

        if (nunchuck->directionPressed(RIGHT)) {
            // At symbol `@`
            Keyboard.press(MODIFIERKEY_RIGHT_SHIFT);
            Keyboard.press(KEY_2);
        } else if (nunchuck->directionReleased(RIGHT)) {
            Keyboard.release(KEY_2);
            Keyboard.release(MODIFIERKEY_RIGHT_SHIFT);
        }

        if (nunchuck->directionPressed(LEFT)) {
            // Hash symbol `#`
            Keyboard.press(MODIFIERKEY_RIGHT_SHIFT);
            Keyboard.press(KEY_3);
        } else if (nunchuck->directionReleased(LEFT)) {
            Keyboard.release(KEY_3);
            Keyboard.release(MODIFIERKEY_RIGHT_SHIFT);
        }
    } else {
        if (nunchuck->directionPressed(UP)) {
            Keyboard.press(KEY_PERIOD);
        } else if (nunchuck->directionReleased(UP)) {
            Keyboard.release(KEY_PERIOD);
        }

        if (nunchuck->directionPressed(DOWN)) {
            Keyboard.press(KEY_COMMA);
        } else if (nunchuck->directionReleased(DOWN)) {
            Keyboard.release(KEY_COMMA);
        }

        if (nunchuck->directionPressed(RIGHT)) {
            Keyboard.press(KEY_QUOTE);
        } else if (nunchuck->directionReleased(RIGHT)) {
            Keyboard.release(KEY_QUOTE);
        }

        if (nunchuck->directionPressed(LEFT)) {
            // Double quotes `"`
            Keyboard.press(MODIFIERKEY_LEFT_SHIFT);
            Keyboard.press(KEY_QUOTE);
        } else if (nunchuck->directionReleased(LEFT)) {
            Keyboard.release(KEY_QUOTE);
            Keyboard.release(MODIFIERKEY_LEFT_SHIFT);
        }
    }
}

void KeyboardController::handleKeyboardExitMode() {
    if (nunchuck->buttonZPressed() && !nunchuck->getCurrentInput().buttonC) {
        setActive(false);
    }

    if (nunchuck->buttonCPressed() && !nunchuck->getCurrentInput().buttonZ) {
        Keyboard.press(KEY_ESC);
    } else if (nunchuck->buttonCReleased()) {
        Keyboard.release(KEY_ESC);
    }
}

KeyboardController::KeyboardController(NunchuckController *device) {
    nunchuck = device;
}

void KeyboardController::setActive(bool isActive) {
    isActivated = isActive;
}

bool KeyboardController::isActive() const {
    return isActivated;
}
