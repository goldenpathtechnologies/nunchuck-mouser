//
// Created by Daryl G. Wright on 2023-04-06.
// Copyright (c) 2023 Golden Path Technologies Inc. MIT Licence.
//
#include "LedController.h"
#include "Adafruit_DotStar.h"

void LedController::update() {
    if (delayTimer % 20 == 0) {
        statusLed->setPixelColor(0, currentColor);
        statusLed->setBrightness(50);
        statusLed->show();
    }

    if (activityLedOn && delayTimer % 20 == 0) {
        digitalWrite(13, HIGH);
    } else {
        digitalWrite(13, LOW);
    }
}

LedController::LedController() {
    currentColor = defaultColor;
    statusLed = new Adafruit_DotStar(NUM_LEDS, DATA_PIN, CLOCK_PIN, DOTSTAR_BGR);
    statusLed->begin();
    statusLed->show();
    activityLedOn = false;
//    digitalWrite(13, HIGH);
}

void LedController::setStatusColor(uint32_t color) {
    statusLed->setPixelColor(0, color);
    statusLed->show();
}

void LedController::resetStatusColor() {
    statusLed->setPixelColor(0, defaultColor);
    statusLed->show();
}

void LedController::enableActivityIndicator() {
    activityLedOn = true;
}

void LedController::disableActivityIndicator() {
    activityLedOn = false;
}
