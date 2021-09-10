#pragma once
#include <Arduino.h>
#include <Adafruit_MCP4725.h>
#include <Wire.h>

#include "Utilities.h"
#include "PinDefinitions.h"

class DacHandler {
   public:
    void init();
    void set(const unsigned int);

   private:
    Adafruit_MCP4725 dac;
};

extern DacHandler dacHandler;