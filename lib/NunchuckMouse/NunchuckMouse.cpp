//
// Created by Daryl G. Wright on 2022-12-22.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//
#include <NunchuckMouse.h>
#include <Arduino.h>

NunchuckMouse::NunchuckMouse() = default;

void NunchuckMouse::processInputs(NunchuckInput *input) {
    prevInput = currInput;
    currInput = *input;

    // TODO: If the prev and curr input is the same for a certain amount of time, put the Keyboard and Mouse to sleep.
    //  When this changes, wake them up.

    updateMode();

    switch (mode) {
        case SELECTION:
            handleSelectionMode();
            break;
        case FREEHAND:
            handleFreehandMode();
            break;
        case KEYBOARD:
            handleKeyboardMode();
            break;
        case SCROLL:
            handleScrollMode();
            break;
        default:
            handleMouseMode();
    }
}

void NunchuckMouse::updateMode() {
    if (selectionModeActivated()) {
        mode = SELECTION;
    } else if (isFreehandMode) {
        mode = FREEHAND;
    } else if (isKeyboardMode) {
        mode = KEYBOARD;
    } else if (scrollModeActivated()) {
        mode = SCROLL;
    } else {
        mode = MOUSE;
    }
}

void NunchuckMouse::begin() { // NOLINT(readability-convert-member-functions-to-static)
    Keyboard.begin();
    Mouse.begin();
}

float NunchuckMouse::getAnalogPercentX() const {
    if (currInput.analogX < LIVE_POS_X && currInput.analogX > LIVE_NEG_X) {
        return 0;
    } else if (currInput.analogX >= LIVE_POS_X) {
        return static_cast<float>((currInput.analogX - LIVE_POS_X) * 100) / static_cast<float>(RANGE_POS_X);
    } else {
        return static_cast<float>((LIVE_NEG_X - currInput.analogX) * -100) / static_cast<float>(RANGE_NEG_X);
    }
}

float NunchuckMouse::getAnalogPercentY() const {
    if (currInput.analogY < LIVE_POS_Y && currInput.analogY > LIVE_NEG_Y) {
        return 0;
    } else if (currInput.analogY >= LIVE_POS_Y) {
        return static_cast<float>((currInput.analogY - LIVE_POS_Y) * 100) / static_cast<float>(RANGE_POS_Y);
    } else {
        return static_cast<float>((LIVE_NEG_Y - currInput.analogY) * -100) / static_cast<float>(RANGE_NEG_Y);
    }
}

int NunchuckMouse::getDirectionX() const {
    if (prevInput.analogX == 0) return 0;

    if (currInput.analogX < LIVE_POS_X && currInput.analogX > LIVE_NEG_X) {
        return 0;
    } else if (currInput.analogX >= LIVE_POS_X) {
        return 1;
    } else {
        return -1;
    }
}

int NunchuckMouse::getDirectionY() const {
    if (prevInput.analogY == 0) return 0;

    if (currInput.analogY < LIVE_POS_Y && currInput.analogY > LIVE_NEG_Y) {
        return 0;
    } else if (currInput.analogY >= LIVE_POS_Y) {
        return -1;
    } else {
        return 1;
    }
}

bool NunchuckMouse::isMoving() const {
    return getDirectionX() != 0 || getDirectionY() != 0;
}

void NunchuckMouse::printInputs(Stream &stream) {
    stream.print("X: ");
    stream.print(getAnalogPercentX());
    stream.print(" | Y: ");
    stream.print(getAnalogPercentY());
    stream.print(" | Roll angle: ");
    stream.print(currInput.rollAngle);
    stream.print(" | Pitch angle: ");
    stream.print(currInput.pitchAngle);
    stream.print(" | Mode: ");
    switch (mode) {
        case SCROLL:
            stream.print("SCROLL");
            break;
        case FREEHAND:
            stream.print("FREEHAND");
            break;
        case KEYBOARD:
            stream.print("KEYBOARD");
            break;
        case SELECTION:
            stream.print("SELECTION");
            break;
        default:
            stream.print("MOUSE");
    }
    stream.println();
}

bool NunchuckMouse::rollAngleInRange(float a0, float a1) const {
    return angleInRange(currInput.rollAngle, a0, a1);
}

bool NunchuckMouse::pitchAngleInRange(float a0, float a1) const {
    return angleInRange(currInput.pitchAngle, a0, a1);
}

bool NunchuckMouse::angleInRange(float value, float a0, float a1) {
    if (a0 == a1) return false;

    if (a0 < a1) {
        return value > a0 && value < a1;
    } else {
        return (value > a0 && value < 180) || (value > -180 && value < a1);
    }
}

bool NunchuckMouse::isSideTilted() {
    return (rollAngleInRange(40, 120) || rollAngleInRange(-120, -40)) && pitchAngleInRange(-35, 35);
}

bool NunchuckMouse::isTiltedUp() {
    return pitchAngleInRange(35, 90);
}

bool NunchuckMouse::isTiltedDown() {
    return pitchAngleInRange(-90, -35);
}

bool NunchuckMouse::scrollModeActivated() {
    return isSideTilted() && (mode == MOUSE || mode == SCROLL);
}

void NunchuckMouse::handleMouseMode() {
    auto xMovement = static_cast<int8_t>(getDirectionX() * getPrecision(getAnalogPercentX()));
    auto yMovement = static_cast<int8_t>(getDirectionY() * getPrecision(getAnalogPercentY()));

    if (isMoving()) {
        Mouse.move(xMovement, yMovement);
    }

    if (currInput.buttonZ) {
        Mouse.press();
    } else {
        Mouse.release();
    }

    if (currInput.buttonC) {
        Mouse.press(MOUSE_RIGHT);
    } else {
        Mouse.release(MOUSE_RIGHT);
    }
}

int NunchuckMouse::getPrecision(float analogPercentage) {
    float data = abs(analogPercentage);

    if (data < 30) {
        return 1;
    } else if (data < 40) {
        return 2;
    } else if (data < 50) {
        return 3;
    } else if (data < 60) {
        return 4;
    } else if (data < 70) {
        return 5;
    } else if (data < 80) {
        return 6;
    } else {
        return 7;
    }
}

void NunchuckMouse::handleScrollMode() {
    auto xMovement = static_cast<int8_t>(getDirectionX() * getScrollPrecision(getAnalogPercentX()));
    auto yMovement = static_cast<int8_t>(getDirectionY() * getScrollPrecision(getAnalogPercentY()));

    if (yMovement != 0 && scrollDelay > 100) {
        Mouse.scroll(yMovement);
        scrollDelay = 0;
    } else if (xMovement != 0 && scrollDelay > 50) {
        Mouse.scroll(0, xMovement);
        scrollDelay = 0;
    }

    if (currInput.buttonC) {
        Mouse.press(MOUSE_MIDDLE);
    } else {
        Mouse.release(MOUSE_MIDDLE);
    }
}

int NunchuckMouse::getScrollPrecision(float analogPercentage) {
    float data = abs(analogPercentage);

    if (data < 50) {
        return 1;
    } else if (data < 70) {
        return 2;
    } else {
        return 3;
    }

    // TODO: If scrolling occurs for more than a few seconds, increase the precision by a lot.
}

void NunchuckMouse::handleFreehandMode() {
//    auto xMovement = static_cast<int8_t>(getFreehandPrecision(currInput.rollAngle));
//    auto yMovement = static_cast<int8_t>(-1 * getFreehandPrecision(currInput.pitchAngle));

    if (currInput.buttonC) {
        isFreehandMode = false;
    }
    // TODO: Disabling movement in freehand mode until the algorithm is refined. Using roll and pitch angle is unwieldy.
    //  I'll have to use the accelX, accelY, and accelZ values directly in an equation.
    //Mouse.move(xMovement, yMovement);
}

int NunchuckMouse::getFreehandPrecision(float analogPercentage) {
//    float data = abs(analogPercentage);
//    int precision = 0;
//    int direction = static_cast<int>(analogPercentage / data);
//
//    // Rolling dead zone -15 to 15
//    // Max roll +/- 55
//    // Pitch dead zone -20 to 20
//    // Max pitch +/- 60
//    // Greater than max has no movement, which allows for selection mode with given numbers
//
//    if (data < 15) {
//        precision = 0;
//    } else if (data < 25) {
//        precision = 1;
//    } else if (data < 30) {
//        precision = 2;
//    } else if (data < 35) {
//        precision = 3;
//    } else if (data < 40) {
//        precision = 4;
//    } else if (data < 45) {
//        precision = 5;
//    } else if (data < 50) {
//        precision = 6;
//    } else if (data < 55) {
//        precision = 7;
//    }
//
//    return precision * direction;
    return 0;
}

void NunchuckMouse::handleSelectionMode() {
    int yDirection = getDirectionY();

    if (yDirection == -1) {
        isFreehandMode = true;
    } else if (yDirection == 1) {
        isKeyboardMode = true;
    }
}

bool NunchuckMouse::selectionModeActivated() {
    return pitchAngleInRange(60, 115) && mode != FREEHAND;
}

void NunchuckMouse::handleKeyboardMode() {
    updateKeyboardMode();
    updateDirectionalState();

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

void NunchuckMouse::updateKeyboardMode() {
    if (isSideTilted()) {
        keyboardMode = NAVIGATING;
    } else if (isTiltedUp()) {
        keyboardMode = QUICK_KEYS;
    } else if (isTiltedDown()) {
        keyboardMode = EXIT;
    } else {
        keyboardMode = TYPING;
    }
}

void NunchuckMouse::handleKeyboardTypingMode() {

}

void NunchuckMouse::handleKeyboardNavigatingMode() {
    if (getDirectionX() > 0) {
        Keyboard.press(KEY_RIGHT_ARROW);
    } else {
        Keyboard.release(KEY_RIGHT_ARROW);
    }

    if (getDirectionX() < 0) {
        Keyboard.press(KEY_LEFT_ARROW);
    } else {
        Keyboard.release(KEY_LEFT_ARROW);
    }

    if (getDirectionY() < 0) {
        Keyboard.press(KEY_UP_ARROW);
    } else {
        Keyboard.release(KEY_UP_ARROW);
    }

    if (getDirectionY() > 0) {
        Keyboard.press(KEY_DOWN_ARROW);
    } else {
        Keyboard.release(KEY_DOWN_ARROW);
    }

    if (currInput.buttonZ && !currInput.buttonC) {
        Keyboard.press(KEY_ENTER);
    } else {
        Keyboard.release(KEY_ENTER);
    }

    if (currInput.buttonC && !currInput.buttonZ) {
        Keyboard.press(KEY_BACKSPACE);
    } else {
        Keyboard.release(KEY_BACKSPACE);
    }
}

void NunchuckMouse::handleKeyboardQuickKeysMode() {
    if (currInput.buttonZ) {
        Keyboard.press(KEY_SPACE);
    } else {
        Keyboard.release(KEY_SPACE);
    }

    if (isActiveDirection(UP)) {
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

void NunchuckMouse::handleKeyboardExitMode() {
    if (currInput.buttonZ && !currInput.buttonC) {
        isKeyboardMode = false;
    }

    if (currInput.buttonC && !currInput.buttonZ) {
        Keyboard.press(KEY_ESC);
    } else {
        Keyboard.release(KEY_ESC);
    }
}

int NunchuckMouse::getDigitalDirection() {
    int direction = NONE;

    if (getAnalogPercentX() > 95) {
        direction |= RIGHT;
    } else if (getAnalogPercentX() < -95) {
        direction |= LEFT;
    }

    if (getAnalogPercentY() > 95) {
        direction |= UP;
    } else if (getAnalogPercentY() < -95) {
        direction |= DOWN;
    }

    return direction;
}

bool NunchuckMouse::isActiveDirection(int direction) {
    return (getDigitalDirection() & direction) == direction;
}

void NunchuckMouse::updateDirectionalState() {
    prevDirectionalState = currDirectionalState;

    currDirectionalState = getDigitalDirection();
}
