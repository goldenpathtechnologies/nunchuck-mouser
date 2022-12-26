//
// Created by Daryl G. Wright on 2022-12-20.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//
#include <main.h>
#include <NunchuckMouse.h>
#include <WiiChuck.h>

Accessory nunchuck;
NunchuckInput input{};
NunchuckMouse mouse;

void setup() {
    nunchuck.begin();
    mouse.begin();

    if (nunchuck.type == Unknown) {
        nunchuck.type = NUNCHUCK;
    }

    readInputs(&nunchuck, &input);
}

void loop() {
    nunchuck.readData();
    readInputs(&nunchuck, &input);
    mouse.processInputs(&input);
//    mouse.printInputs();
}

void readInputs(Accessory *c, NunchuckInput *i) {
    i->analogX = c->getJoyX();
    i->analogY = c->getJoyY();
    i->pitchAngle = c->getPitchAngle();
    i->rollAngle = c->getRollAngle();
    i->accelX = c->getAccelX();
    i->accelY = c->getAccelY();
    i->accelZ = c->getAccelZ();
    i->buttonC = c->getButtonC();
    i->buttonZ = c->getButtonZ();
}
