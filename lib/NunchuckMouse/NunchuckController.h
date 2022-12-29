//
// Created by Daryl G. Wright on 2022-12-29.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//

#ifndef NUNCHUCK_MOUSER_NUNCHUCKCONTROLLER_H
#define NUNCHUCK_MOUSER_NUNCHUCKCONTROLLER_H
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

enum NunchuckMode {
    MOUSE,
    SCROLL,
    SELECTION,
    FREEHAND,
    KEYBOARD
};

enum DirectionalButtons {
    NONE = 0x0000,
    UP = 0x0001,
    DOWN = 0x0010,
    LEFT = 0x0100,
    RIGHT = 0x1000
};

class NunchuckController {
public:
    int getDirectionX() const;
    int getDirectionY() const;
    bool isMoving() const;
    float getAnalogPercentX() const;
    float getAnalogPercentY() const;
    static bool angleInRange(float value, float a0, float a1);
    bool rollAngleInRange(float a0, float a1) const;
    bool pitchAngleInRange(float a0, float a1) const;
    bool isSideTilted() const;
    bool isTiltedUp() const;
    bool isTiltedDown() const;
    void updateDirectionalState();
    bool directionalStateChanged() const;
    int getDigitalDirection() const;
    bool isActiveDirection(int direction) const;
    bool directionPressed(int direction) const;
    bool directionReleased(int direction) const;
    bool buttonZPressed();
    bool buttonZReleased();
    bool buttonCPressed();
    bool buttonCReleased();
    NunchuckInput getCurrentInput();
    NunchuckInput getPreviousInput();
    void updateInput(NunchuckInput *input);
    NunchuckMode getMode();
    void setMode(NunchuckMode m);
private:
    NunchuckInput currInput{};
    NunchuckInput prevInput{};
    NunchuckMode mode = MOUSE;
    int prevDirectionalState = NONE;
    int currDirectionalState = NONE;
};
#endif //NUNCHUCK_MOUSER_NUNCHUCKCONTROLLER_H
