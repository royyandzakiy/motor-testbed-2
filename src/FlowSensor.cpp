#include "FlowSensor.h"

FlowSensor flowSensor;

void IRAM_ATTR flowtickIsr() {
    flowSensor.tick();
}

void FlowSensor::init() {
    pinMode(FLOWSENSOR_PIN, INPUT);
    digitalWrite(FLOWSENSOR_PIN, HIGH);

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
 * @return debit in L/s
 */
unsigned int FlowSensor::get() {
    unsigned long elapsedTime = millis() - debitStart;
    unsigned int debit = (debitTick / 7.5) * debitConstant / 60.0 / elapsedTime;  // debit formula, L/s
    return debit;
}