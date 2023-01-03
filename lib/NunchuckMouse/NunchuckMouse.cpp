//
// Created by Daryl G. Wright on 2022-12-22.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//

#include <NunchuckMouse.h>
#include <Arduino.h>

NunchuckMouse::NunchuckMouse() {
    nunchuck = new NunchuckController();
    mouse = new MouseController(nunchuck);
    scroller = new ScrollController(nunchuck);
    joystick = new JoystickController(nunchuck);
    keyboard = new KeyboardController(nunchuck);
    selector = new SelectionController(nunchuck, joystick, keyboard);
};

void NunchuckMouse::processInputs(NunchuckInput *input) {
    nunchuck->updateInput(input);
    nunchuck->updateDirectionalState();

    // TODO: If the prev and curr input is the same for a certain amount of time, put the Keyboard and Mouse to sleep.
    //  When this changes, wake them up.

    updateMode();

    switch (nunchuck->getMode()) {
        case SELECTION:
            selector->handle();
            break;
        case JOYSTICK:
            joystick->handle();
            break;
        case KEYBOARD:
            keyboard->handle();
            break;
        case SCROLL:
            scroller->handle();
            break;
        default:
            mouse->handle();
    }
}

void NunchuckMouse::updateMode() {
    if (joystick->isActive()) {
        nunchuck->setMode(JOYSTICK);
    } else if (keyboard->isActive()) {
        nunchuck->setMode(KEYBOARD);
    } else if (selector->isActive()) {
        // Note: All modes that SELECTION switches to must be checked before this, or SELECTION mode will
        //  remain active until manually deactivated.
        nunchuck->setMode(SELECTION);
    } else if (scroller->isActive()) {
        nunchuck->setMode(SCROLL);
    } else {
        nunchuck->setMode(MOUSE);
    }
}

void NunchuckMouse::begin() { // NOLINT(readability-convert-member-functions-to-static)
    Keyboard.begin();
    Mouse.begin();
}

void NunchuckMouse::printInputs(Stream &stream) {
    stream.print("X: ");
    stream.print(nunchuck->getAnalogPercentX());
    stream.print(" | Y: ");
    stream.print(nunchuck->getAnalogPercentY());
    stream.print(" | Roll angle: ");
    stream.print(nunchuck->getCurrentInput().rollAngle);
    stream.print(" | Pitch angle: ");
    stream.print(nunchuck->getCurrentInput().pitchAngle);
    stream.print(" | Mode: ");
    switch (nunchuck->getMode()) {
        case SCROLL:
            stream.print("SCROLL");
            break;
        case JOYSTICK:
            stream.print("JOYSTICK");
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
