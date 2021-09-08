#include "Sampler.h"

Sampler sampler;
TaskHandle_t samplingTaskHandle;

float* rawSampling();
void sampling(void*);

void Sampler::init() {
    xTaskCreate(sampling, "sampling", 2048, NULL, 1, NULL);
}

#define LOADCELL_AMOUNT 4
uint16_t adcRead[LOADCELL_AMOUNT];
unsigned int rawSampleCount = 0;

const unsigned long rawSampleCountMax = 6000;
const unsigned long rawSampleInterval = 0;       // interval from each raw sampling (in microseconds). need to create dynamically, count total sample count, reduce by estimated sisa waktu. default is 0
const unsigned long rawSampleTimeMax = 1000000;  // max raw sampling time is 1 second (in microseconds)

float* Sampler::rawSampling() {
    unsigned long start = micros();
    bool stopSampling;

    while (1) {
        stopSampling = rawSampleCount >= rawSampleCountMax;  // stop based on buffer size
        // stopSampling = micros() - start > rawSampleTimeMax; // stop based on time
        // stopSampling = rawSampleCount >= rawSampleCountMax || micros() - start > rawSampleTimeMax; // stop based on buffer size and time

        if (stopSampling) {
            break;
        }

        adcRead[0] += adc1_get_raw(ADC1_CHANNEL_4);
        adcRead[1] += adc1_get_raw(ADC1_CHANNEL_5);
        adcRead[2] += adc1_get_raw(ADC1_CHANNEL_6);
        adcRead[3] += adc1_get_raw(ADC1_CHANNEL_7);

        delayMicroseconds(rawSampleInterval);
        rawSampleCount++;
    }

    float* avgSample = (float*)malloc(sizeof(float) * LOADCELL_AMOUNT);

    for (int i = 0; i < 4; i++) {
        avgSample[i] = (float)(adcRead[i] / rawSampleCount);
    }

    _PF("rawSampleCount: %d\n", rawSampleCount);
    _PF("time elapsed: %lu\n", micros() - start);

    return avgSample;
}

unsigned int avgSampleCount = 0;

const unsigned int avgSampleCountMax = 50;         // is "buffer size"
const unsigned long avgSampleInterval = 500000;   // in microseconds
const unsigned long avgSampleTimeMax = 10000000;  // an alternative to not stop by buffer size, but time (in microseconds)

// ROY: START HERE, perbaiki sampling
void samplingTask(void* pvParameters) {
    bool stopSampling;
    unsigned long start = micros();

    for (;;) {
        stopSampling = avgSampleCount >= avgSampleCountMax;  // stop based on buffer size
        // stopSampling = micros() - start > avgSampleTimeMax; // stop based on time
        // stopSampling = avgSampleCount >= avgSampleCountMax || micros() - start > avgSampleTimeMax; // stop based on buffer size & time

        if (stopSampling) {
            break;
        }

        float* avgSample;
        avgSample = sampler.rawSampling();

        avgSampleCount++;
        
        _PF("sampling count: %d\n", avgSampleCount);
        for (int i = 0; i < LOADCELL_AMOUNT; i++) {
            _PF("average adcRead[%d]: %.2f\n", i, avgSample[i]);
        }
        delayMicroseconds(avgSampleInterval);
    }

    _PF("avgSampleCount: %d\n", avgSampleCount);
    _PF("time elapsed: %lu\n", micros() - start);
    vTaskDelete(NULL);
}