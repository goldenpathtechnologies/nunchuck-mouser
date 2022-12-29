//
// Created by Daryl G. Wright on 2022-12-29.
// Copyright (c) 2022 Golden Path Technologies Inc. MIT Licence.
//

#ifndef NUNCHUCK_MOUSER_FREEHANDCONTROLLER_H
#define NUNCHUCK_MOUSER_FREEHANDCONTROLLER_H
#include "NunchuckController.h"

class FreehandController {
public:
    explicit FreehandController(NunchuckController *device);

    void handle();
    bool isActive() const;
    void setActive(bool isActive);
private:
    NunchuckController *nunchuck;
    bool isActivated = false;
    static int getPrecision(float analogPercentage);
};
#endif //NUNCHUCK_MOUSER_FREEHANDCONTROLLER_H
