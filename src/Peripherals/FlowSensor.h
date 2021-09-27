#pragma once
#include <Arduino.h>

#include "PinDefinitions.h"
#include "Utilities.h"

class FlowSensor {
   public:
    void init(const char * _name, uint8_t);
    void tick();
    void reset();
    void flowOutTickIsr();
    float get();

   private:
    const char * name;
    uint8_t pin;

    unsigned long debitStart;
    bool firstTick;
    unsigned long debitTick;
    float debitConstant = 2.6;  // Konstanta flow sensor
};

extern void flowOutTickIsr();
extern void flowInTickIsr();
extern FlowSensor flowSensorOut;
extern FlowSensor flowSensorIn;