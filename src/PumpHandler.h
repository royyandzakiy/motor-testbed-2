#pragma once
#include <Arduino.h>
#include "Utilities.h"
#include "PinDefinitions.h"
#include "FlowSensor.h"

class PumpHandler {
    public:
        void init(const char *, uint8_t);
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
        
        bool state;

    private:
        const char * name;
        uint8_t pin;
        unsigned int dutyCycle;

        float kp;
        float ki;
        float debitTarget;
        
        float error;
        float errorSum;
};

extern PumpHandler pumpIn;
extern PumpHandler pumpOut;