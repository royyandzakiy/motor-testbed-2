#pragma once
#include <Arduino.h>
#include "PinDefinitions.h"

class PumpHandler {
    public:
        PumpHandler();
        void set(const unsigned int);
};

extern PumpHandler pumpHandler;