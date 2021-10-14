#pragma once
#include <Arduino.h>

class Sensor {
    public:
        virtual float * avgSampling();
        virtual float read();
};