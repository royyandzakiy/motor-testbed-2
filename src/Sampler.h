#pragma once

#include <Arduino.h>
#include "Utilities.h"

class Sampler {
    public:
        void init();
        void samplerTask(void*);
        void poll();
};

extern Sampler sampler;