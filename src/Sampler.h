#pragma once
#include <Arduino.h>

#include "Utilities.h"
#include "driver/adc.h"
#include "driver/gpio.h"
#include "esp_adc_cal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

typedef struct {
    // --------- RAW SAMPLING ---------
    unsigned long rawSampleInterval;        // interval from each raw sampling (in microseconds). need to create dynamically, count total sample count, reduce by estimated sisa waktu. default is 0

    // --------- AVERAGE SAMPLING ---------
    unsigned long avgSampleInterval;        // in microseconds
    char avgSamplingStopMode[30];           // options: buffer, duration, timestamp; and combinations of 3 options (eg: bufferduration)
    unsigned int avgSamplingBufferSize;     // is buffer size"
    unsigned long avgSamplingDurationMax;   // max raw sampling time is 1 second (in microseconds)

    // --------- TOTAL SAMPLING ---------
    unsigned int totalSamplingBufferSize;   // is "buffer size"
    char totalSamplingStopMode[30];         // options: buffer, duration, timestamp; and combinations of 3 options (eg: bufferduration)
    unsigned long totalSampleDurationMax;   // an alternative to not stop by buffer size, but time (in microseconds)
} SamplerConfig_t;

class Sampler {
   public:
    void init();
    float * avgSampling();

    void start();
    void stop();

    void set(const char *, const char *);
    void printConfiguration();

    bool state;

    SamplerConfig_t config;
};

extern void samplingTask(void*);
extern TaskHandle_t samplingTaskHandle;

extern Sampler sampler;