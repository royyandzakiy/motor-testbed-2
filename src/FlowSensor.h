#pragma once
#include <Arduino.h>
#include "PinDefinitions.h"

class FlowSensor {
    public:
        void init();
        void tick();
        void reset();
        void flowtickIsr();
        unsigned int get();

    private:
        unsigned long debitStart;
        bool firstTick;
        unsigned long debitTick;
        float debitConstant = 2.6; // Konstanta flow sensor
};

extern void flowtickIsr();
extern FlowSensor flowSensor;