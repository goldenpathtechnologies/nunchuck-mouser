//
// Created by Daryl G. Wright on 2022-12-22.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//

#ifndef NUNCHUCK_MOUSER_NUNCHUCKMOUSE_H
#define NUNCHUCK_MOUSER_NUNCHUCKMOUSE_H

#include <Stream.h>
#include <Arduino.h>
#include <Constants.h>

class NunchuckMouse {
public:
    NunchuckMouse();
    void processInputs(NunchuckInput *input);
    void begin();
    void printInputs(Stream &stream = Serial);

private:
    NunchuckMode mode = MOUSE;
    NunchuckInput currInput{};
    NunchuckInput prevInput{};
    // TODO: Classes will be as follows:
    //  NunchuckController device;
    //  MouseController mouse;
    //  ScrollController scroller;
    //  SelectionController selector;
    //  FreehandController freehandMouse;
    //  KeyboardController keyboard;
    //  With the exception of the NunchuckController, each of these classes will take an instance of NunchuckController
    //  as a constructor argument. This of course is a pointer to a single NunchuckController instance. The
    //  NunchuckMouse class will create instances of all these classes and manage the communication and coordination
    //  between them.

    void updateMode();

    // NunchuckController
    int getDirectionX() const;
    int getDirectionY() const;
    bool isMoving() const;
    float getAnalogPercentX() const;
    float getAnalogPercentY() const;
    static bool angleInRange(float value, float a0, float a1);
    bool rollAngleInRange(float a0, float a1) const;
    bool pitchAngleInRange(float a0, float a1) const;
    bool isSideTilted();
    bool isTiltedUp();
    bool isTiltedDown();
    int prevDirectionalState = NONE;
    int currDirectionalState = NONE;
    void updateDirectionalState();
    int getDigitalDirection();
    bool isActiveDirection(int direction);

    // MouseController
    void handleMouseMode();
    static int getPrecision(float analogPercentage);

    // ScrollController
    bool scrollModeActivated();
    void handleScrollMode();
    static int getScrollPrecision(float analogPercentage);
    elapsedMillis scrollDelay;

    // FreehandController
    void handleFreehandMode();
    static int getFreehandPrecision(float analogPercentage);
    bool isFreehandMode = false;

    // SelectionController
    void handleSelectionMode();
    bool selectionModeActivated();

    // KeyboardController
    bool isKeyboardMode = false;
    void handleKeyboardMode();
    KeyboardMode keyboardMode = TYPING;
    void updateKeyboardMode();
    void handleKeyboardTypingMode();
    void handleKeyboardNavigatingMode();
    void handleKeyboardQuickKeysMode();
    void handleKeyboardExitMode();
};
#endif //NUNCHUCK_MOUSER_NUNCHUCKMOUSE_H
