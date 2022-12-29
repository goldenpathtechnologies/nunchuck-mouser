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
    if (nunchuck->getDirectionX() > 0) {
        Keyboard.press(KEY_RIGHT_ARROW);
    } else {
        Keyboard.release(KEY_RIGHT_ARROW);
    }

    if (nunchuck->getDirectionX() < 0) {
        Keyboard.press(KEY_LEFT_ARROW);
    } else {
        Keyboard.release(KEY_LEFT_ARROW);
    }

    if (nunchuck->getDirectionY() < 0) {
        Keyboard.press(KEY_UP_ARROW);
    } else {
        Keyboard.release(KEY_UP_ARROW);
    }

    if (nunchuck->getDirectionY() > 0) {
        Keyboard.press(KEY_DOWN_ARROW);
    } else {
        Keyboard.release(KEY_DOWN_ARROW);
    }

    if (nunchuck->getCurrentInput().buttonZ && !nunchuck->getCurrentInput().buttonC) {
        Keyboard.press(KEY_ENTER);
    } else {
        Keyboard.release(KEY_ENTER);
    }

    if (nunchuck->getCurrentInput().buttonC && !nunchuck->getCurrentInput().buttonZ) {
        Keyboard.press(KEY_BACKSPACE);
    } else {
        Keyboard.release(KEY_BACKSPACE);
    }
}

void KeyboardController::handleKeyboardQuickKeysMode() {
    if (nunchuck->getCurrentInput().buttonZ) {
        Keyboard.press(KEY_SPACE);
    } else {
        Keyboard.release(KEY_SPACE);
    }

    if (nunchuck->isActiveDirection(UP)) {
        // TODO: The problem here is that the nunchuck is calling Keyboard.press() several times. We only want to do this
        //  once when the state changes, and release when the state changes again. I'll have to track the state of the
        //  digital directional buttons to accomplish all of the above. Additionally, I'll have to fix all the other
        //  modes to only press buttons when state changes.
        Keyboard.press(KEY_PERIOD);
    } else {
        Keyboard.release(KEY_PERIOD);
    }

//    if (currInput.buttonC) {
//        if (directionUp) {
//            Keyboard.press(KEY_MINUS);
//        } else {
//            Keyboard.release(KEY_MINUS);
//        }
//
//        if (directionDown) {
//            // Question mark `?`
//            Keyboard.press(MODIFIERKEY_LEFT_SHIFT);
//            Keyboard.press(KEY_SLASH);
//        } else {
//            Keyboard.release(MODIFIERKEY_LEFT_SHIFT);
//            Keyboard.release(KEY_SLASH);
//        }
//
//        if (directionRight) {
//            // At symbol `@`
//            Keyboard.press(MODIFIERKEY_RIGHT_SHIFT);
//            Keyboard.press(KEY_2);
//        } else {
//            Keyboard.release(MODIFIERKEY_RIGHT_SHIFT);
//            Keyboard.release(KEY_2);
//        }
//
//        if (directionLeft) {
//            // Hash symbol `#`
//            Keyboard.press(MODIFIERKEY_RIGHT_SHIFT);
//            Keyboard.press(KEY_3);
//        } else {
//            Keyboard.release(MODIFIERKEY_RIGHT_SHIFT);
//            Keyboard.release(KEY_3);
//        }
//    } else {
//        if (directionUp) {
//            Keyboard.press(KEY_PERIOD);
//        } else {
//            Keyboard.release(KEY_PERIOD);
//        }
//
//        if (directionDown) {
//            Keyboard.press(KEY_COMMA);
//        } else {
//            Keyboard.release(KEY_COMMA);
//        }
//
//        if (directionRight) {
//            Keyboard.press(KEY_QUOTE);
//        } else {
//            Keyboard.release(KEY_QUOTE);
//        }
//
//        if (directionLeft) {
//            // Double quotes `"`
//            Keyboard.press(MODIFIERKEY_LEFT_SHIFT);
//            Keyboard.press(KEY_QUOTE);
//        } else {
//            Keyboard.release(MODIFIERKEY_LEFT_SHIFT);
//            Keyboard.release(KEY_QUOTE);
//        }
//    }
}

void KeyboardController::handleKeyboardExitMode() {
    if (nunchuck->getCurrentInput().buttonZ && !nunchuck->getCurrentInput().buttonC) {
        setActive(false);
    }

    if (nunchuck->getCurrentInput().buttonC && !nunchuck->getCurrentInput().buttonZ) {
        Keyboard.press(KEY_ESC);
    } else {
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
