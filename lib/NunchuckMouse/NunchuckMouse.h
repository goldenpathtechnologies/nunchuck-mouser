//
// Created by Daryl G. Wright on 2022-12-22.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//

#ifndef NUNCHUCK_MOUSER_NUNCHUCKMOUSE_H
#define NUNCHUCK_MOUSER_NUNCHUCKMOUSE_H

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
    short getDirectionX() const;
    short getDirectionY() const;
    bool isMoving() const;
    float getAnalogPercentX() const;
    float getAnalogPercentY() const;
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
    NunchuckInput currInput{};
    NunchuckInput prevInput{};
};
#endif //NUNCHUCK_MOUSER_NUNCHUCKMOUSE_H
