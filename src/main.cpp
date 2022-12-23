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
    printInputs();
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

void printInputs() {
//    Serial.printf("NUNCHUCK   JoyX: %4d  | JoyY: %4d | Ax: %4d | Ay: %4d | Az: %4d | Buttons: ",
//                  nunchuck.getJoyX(), nunchuck.getJoyY(), nunchuck.getAccelX(), nunchuck.getAccelY(),
//                  nunchuck.getAccelZ());
//    if (nunchuck.getButtonC()) {
//        Serial.print("C");
//    } else {
//        Serial.print("-");
//    }
//
//    if (nunchuck.getButtonZ()) {
//        Serial.print("Z");
//    } else {
//        Serial.print("-");
//    }
//
//    Serial.print(" | Roll angle: ");
//    Serial.print(nunchuck.getRollAngle());
//    Serial.print(" | Pitch angle: ");
//    Serial.print(nunchuck.getPitchAngle());
//    Serial.println();

    Serial.print("X: ");
    Serial.print(mouse.getAnalogPercentX());
    Serial.print(" | Y: ");
    Serial.print(mouse.getAnalogPercentY());
    Serial.println();
}
