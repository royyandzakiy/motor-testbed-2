#pragma once
#include <Arduino.h>

#include "Utilities.h"
#include "driver/adc.h"
#include "driver/gpio.h"
#include "esp_adc_cal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class Sampler {
   public:
    void init();
    float * avgSampling();

    void start();
    void stop();

    private:
    bool samplingActive;
};

extern void samplingTask(void*);
extern TaskHandle_t samplingTaskHandle;

extern Sampler sampler;