//
// Created by Daryl G. Wright on 2022-12-22.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//

#include <Arduino.h>
#include <HID-Project.h>
#include "NunchuckMouse.h"

NunchuckMouse::NunchuckMouse() {
    led = new LedController();
    nunchuck = new NunchuckController();
    scroller = new ScrollController(nunchuck);
    keyboard = new KeyboardController(nunchuck, led);
    mouse = new MouseController(nunchuck, keyboard);
}

void NunchuckMouse::processInputs(NunchuckInput *input) {
    nunchuck->updateInput(input);
    nunchuck->updateDirectionalState();

    updateMode();

    if (nunchuck->isMoving()) {
        led->enableActivityIndicator();
    } else {
        led->disableActivityIndicator();
    }

    led->update();

    switch (nunchuck->getMode()) {
        case KEYBOARD:
            keyboard->handle();
            break;
        case SCROLL:
            led->setStatusColor(0xFF5733);
            scroller->handle();
            break;
        default:
            led->resetStatusColor();
            mouse->handle();
    }
}

void NunchuckMouse::updateMode() {
    if (keyboard->isActive()) {
        if (nunchuck->getMode() == MOUSE) Mouse.release(MOUSE_ALL);
        nunchuck->setMode(KEYBOARD);
    } else if (scroller->isActive()) {
        if (nunchuck->getMode() == MOUSE)
            Mouse.release(MOUSE_LEFT | MOUSE_RIGHT);
        nunchuck->setMode(SCROLL);
    } else {
        if (nunchuck->getMode() == SCROLL) {
            Mouse.release(MOUSE_MIDDLE);
        } else if (nunchuck->getMode() == KEYBOARD) {
            Keyboard.releaseAll();
        }
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
        case KEYBOARD:
            stream.print("KEYBOARD");
            break;
        default:
            stream.print("MOUSE");
    }
    stream.println();
}
