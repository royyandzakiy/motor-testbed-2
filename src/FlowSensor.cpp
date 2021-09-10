#include "FlowSensor.h"

FlowSensor florSensorOut;
FlowSensor florSensorIn;

void IRAM_ATTR flowOutTickIsr() {
    florSensorOut.tick();
}

void IRAM_ATTR flowInTickIsr() {
    florSensorIn.tick();
}

void FlowSensor::init(const uint8_t flowSensorPin) {
    pinMode(flowSensorPin, INPUT);
    digitalWrite(flowSensorPin, HIGH);

    reset();
}

void FlowSensor::tick() {
    debitTick++;

    if (firstTick) {
        firstTick = false;
        debitStart = millis();
    }
}

void FlowSensor::reset() {
    debitTick = 0;
    firstTick = true;
}

/***
 * @brief
 * @return debitTarget in L/s
 */
float FlowSensor::get() {
    unsigned long elapsedTime = millis() - debitStart;

    // debitTarget formula, L/s
    float debitTarget = (debitTick / 7.5) * debitConstant / 60.0 / elapsedTime;  

    return debitTarget;
}