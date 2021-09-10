#include "Mcp4725Handler.h"

Mcp4725Handler mcp4725Handler;

// PWM Properties
const int freq = 5000;
const int pumpChannel = 0;
const int resolution = 8;

#define DAC_IDLE_VALUE 90

void Mcp4725Handler::init() {
    dac.begin(0x60);  //I2C address
    set(DAC_IDLE_VALUE);
}

void Mcp4725Handler::set(const unsigned int output) {
    dac.setVoltage(output, false);
}

void voltageTest() {
    for (int i = 0; i < 4096; i++) {
        mcp4725Handler.set(i);
        delay(5);
    }
}