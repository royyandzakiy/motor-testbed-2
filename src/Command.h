#pragma once

#include <Arduino.h>
#include "Utilities.h"
#include "DeviceConfig.h"
#include "Sampler.h"

class Command {
    public:
        void poll();
        void process(const String);
};

extern Command command;