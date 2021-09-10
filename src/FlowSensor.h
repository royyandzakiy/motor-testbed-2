#pragma once
#include <Arduino.h>

#include "PinDefinitions.h"

class FlowSensor {
   public:
    void init(const uint8_t);
    void tick();
    void reset();
    void flowOutTickIsr();
    float get();

   private:
    unsigned long debitStart;
    bool firstTick;
    unsigned long debitTick;
    float debitConstant = 2.6;  // Konstanta flow sensor
};

extern void flowOutTickIsr();
extern void flowInTickIsr();
extern FlowSensor florSensorOut;
extern FlowSensor florSensorIn;