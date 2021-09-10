#include "PumpHandler.h"

PumpHandler pumpHandler;

// PWM Properties
const int freq = 5000;
const int pumpChannel = 0;
const int resolution = 8;

void PumpHandler::init() {
    ledcSetup(pumpChannel, freq, resolution);
    ledcAttachPin(PUMP_RELAY_PIN, pumpChannel);
}

unsigned int PumpHandler::get() {
    return dutyCycle;
}

void PumpHandler::set(const unsigned int _dutyCycle) {
    dutyCycle = _dutyCycle;
    if (state) {
        poll();
    }
}

void PumpHandler::start() {
    state = true;
    poll();
}

void PumpHandler::stop() {
    state = false;
    ledcWrite(pumpChannel, 0);
}

void PumpHandler::poll() {
    ledcWrite(pumpChannel, dutyCycle);
}

void ledTest() {
    for (int dutyCycle = 0; dutyCycle < 256; dutyCycle++) {
        pumpHandler.set(dutyCycle);
        delay(15);
    }
    for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
        pumpHandler.set(dutyCycle);
        delay(15);
    }
}