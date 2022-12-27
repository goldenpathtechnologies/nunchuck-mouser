//
// Created by Daryl G. Wright on 2022-12-22.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//

#ifndef NUNCHUCK_MOUSER_NUNCHUCKMOUSE_H
#define NUNCHUCK_MOUSER_NUNCHUCKMOUSE_H

#include <Stream.h>
#include <Arduino.h>

struct NunchuckInput {
    int analogX;
    int analogY;
    float rollAngle;
    float pitchAngle;
    int accelX;
    int accelY;
    int accelZ;
    bool buttonC;
    bool buttonZ;
};

class NunchuckMouse {
public:
    NunchuckMouse();
    void processInputs(NunchuckInput *input);
    void begin();
    void printInputs(Stream &stream = Serial);

private:
    const int DEAD_ZONE_LENGTH = 20;
    const int CENTER_X = 127;
    const int CENTER_Y = 129;
    const int MIN_X = 25;
    const int MIN_Y = 34;
    const int MAX_X = 228;
    const int MAX_Y = 226;
    const int LIVE_POS_X = CENTER_X + DEAD_ZONE_LENGTH;
    const int LIVE_NEG_X = CENTER_X - DEAD_ZONE_LENGTH;
    const int LIVE_POS_Y = CENTER_Y + DEAD_ZONE_LENGTH;
    const int LIVE_NEG_Y = CENTER_Y - DEAD_ZONE_LENGTH;
    const int RANGE_POS_X = MAX_X - LIVE_POS_X;
    const int RANGE_NEG_X = LIVE_NEG_X - MIN_X;
    const int RANGE_POS_Y = MAX_Y - LIVE_POS_Y;
    const int RANGE_NEG_Y = LIVE_NEG_Y - MIN_Y;
    enum MouseMode {
        MOUSE,
        SCROLL,
        SELECTION,
        FREEHAND,
        KEYBOARD
    };
    MouseMode mode = MOUSE;
    NunchuckInput currInput{};
    NunchuckInput prevInput{};

    int getDirectionX() const;
    int getDirectionY() const;
    bool isMoving() const;
    float getAnalogPercentX() const;
    float getAnalogPercentY() const;
    void updateMode();
    static bool angleInRange(float value, float a0, float a1);
    bool rollAngleInRange(float a0, float a1) const;
    bool pitchAngleInRange(float a0, float a1) const;
    bool scrollModeActivated();
    void handleMouseMode();
    static int getPrecision(float analogPercentage);
    void handleScrollMode();
    static int getScrollPrecision(float analogPercentage);
    elapsedMillis scrollDelay;
    void handleFreehandMode();
    static int getFreehandPrecision(float analogPercentage);
    bool isFreehandMode = false;
    void handleSelectionMode();
    bool selectionModeActivated();
    bool isKeyboardMode = false;
};
#endif //NUNCHUCK_MOUSER_NUNCHUCKMOUSE_H
