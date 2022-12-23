//
// Created by Daryl G. Wright on 2022-12-20.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//
#ifndef NUNCHUCK_MOUSER_MAIN_H
#define NUNCHUCK_MOUSER_MAIN_H
#include <WiiChuck.h>
#include <NunchuckMouse.h>

void readInputs(Accessory *c, NunchuckInput *i);
void printInputs();
#endif //NUNCHUCK_MOUSER_MAIN_H
