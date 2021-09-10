#include "DacHandler.h"

DacHandler dacHandler;

// PWM Properties
const int freq = 5000;
const int pumpChannel = 0;
const int resolution = 8;

#define DAC_IDLE_VALUE 90

void DacHandler::init() {
    _PTN("[DacHandler::init]");
    dac.begin(0x60);  //I2C address
    set(DAC_IDLE_VALUE);
}

void DacHandler::set(const unsigned int output) {
    dac.setVoltage(output, false);
}

void voltageTest() {
    for (int i = 0; i < 4096; i++) {
        dacHandler.set(i);
        delay(5);
    }
}