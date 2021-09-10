#pragma once

#include <Arduino.h>
#include "Utilities.h"
#include "Sampler.h"
#include "Storage/DataSaver.h"
#include "Peripherals/PumpHandler.h"
#include "esp_task_wdt.h"

extern TaskHandle_t commandTaskHandle;

class Command {
    public:
        void init();
        void poll();
        void process(const String);
};

extern void samplingStart();
extern void commandTask(void *);
extern Command serialCommand;