#include "PumpHandler.h"

PumpHandler pumpHandler;

// PWM Properties
const int freq = 5000;
const int pumpChannel = 0;
const int resolution = 8;

PumpHandler::PumpHandler() {
    ledcSetup(pumpChannel, freq, resolution);
    ledcAttachPin(PUMP_RELAY_PIN, pumpChannel);
}

void PumpHandler::set(const unsigned int dutyCycle) {
    ledcWrite(pumpChannel, dutyCycle);
}

void ledTest() {
    for (int dutyCycle=0; dutyCycle<256; dutyCycle++) {
        pumpHandler.set(dutyCycle);
        delay(15);
    }
    for (int dutyCycle=255; dutyCycle>=0; dutyCycle--) {
        pumpHandler.set(dutyCycle);
        delay(15);
    }
}