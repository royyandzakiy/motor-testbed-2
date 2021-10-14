#pragma once
#include <functional>

#include "HX711.h"  //You must have this library in your arduino library folder
#include "Sensor.h"
#include "Utilities.h"

#define DOUT 21
#define CLK 19

extern HX711 scale;

class LoadSensor : public Sensor {
   public:
    LoadSensor(String _name): name(_name) {}
    void init();
    float read();
    float* avgSampling();

    long rawSampleInterval;
    uint16_t sensorValues;
    String name;
};