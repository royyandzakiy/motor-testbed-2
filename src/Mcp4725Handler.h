#pragma once
#include <Arduino.h>
#include <Adafruit_MCP4725.h>
#include <Wire.h>

#include "PinDefinitions.h"

class Mcp4725Handler {
   public:
    void init();
    void set(const unsigned int);

   private:
    Adafruit_MCP4725 dac;
};

extern Mcp4725Handler mcp4725Handler;