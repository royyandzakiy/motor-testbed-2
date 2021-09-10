#pragma once
#include <Arduino.h>
#include "PinDefinitions.h"
#include "FlowSensor.h"

class PumpHandler {
    public:
        void init();
        void set(const unsigned int);
        unsigned int get();
        
        void start();
        void stop();
        void poll();
        void meassure(float);

    private:
        unsigned int dutyCycle;
        bool state;
};

extern PumpHandler pumpHandler;