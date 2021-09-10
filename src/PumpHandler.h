#pragma once
#include <Arduino.h>
#include "Utilities.h"
#include "PinDefinitions.h"
#include "FlowSensor.h"

class PumpHandler {
    public:
        void init(const uint8_t);
        void set(const unsigned int);
        unsigned int get();
        
        void start();
        void stop();
        void poll();
        void meassure(float);
        bool isActive();

        float get(const char *);
        void set(const char *, const char *);
        void printConfiguration();

    private:
        unsigned int dutyCycle;
        bool state;

        float kp;
        float ki;
        float debitTarget;
        
        float error;
        float errorSum;
};

extern PumpHandler pumpIn;
extern PumpHandler pumpOut;