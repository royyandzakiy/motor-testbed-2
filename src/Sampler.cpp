#include "Sampler.h"

Sampler sampler;
TaskHandle_t samplingTaskHandle;

#define LOADCELL_AMOUNT 4

/**
 * --------- SAMPLING CONFIGURATIONS ---------
 */
#define SAMPLING_VERBOSE

void Sampler::init() {
    _PTN("[Sampler::init]");
    // --------- RAW SAMLING ---------
    config.rawSampleInterval = 0;  // interval from each raw sampling (in microseconds). need to create dynamically, count total sample count, reduce by estimated sisa waktu. default is 0

    // --------- AVERAGE SAMPLING ---------
    config.avgSampleInterval = 500000;  // in microseconds
    // config.avgSamplingStopMode = "bufferduration";
    memcpy(config.avgSamplingStopMode, "bufferduration", strlen("bufferduration") + 1);
    config.avgSamplingBufferSize = 20;        // is buffer size"
    config.avgSamplingDurationMax = 1000000;  // max raw sampling time is 1 second (in microseconds)

    // --------- TOTAL SAMPLING ---------
    // config.totalSamplingStopMode = "buffer";
    memcpy(config.totalSamplingStopMode, "buffer", strlen("buffer") + 1);
    config.totalSamplingBufferSize = 20000;    // is "buffer size"
    config.totalSampleDurationMax = 10000000;  // an alternative to not stop by buffer size, but time (in microseconds)
}

void Sampler::start() {
    _PTN("[Sampler::start]");

    if (samplingTaskHandle != NULL) {
        _PTN("    | Sampling already running.");
    } else {
        state = true;
        xTaskCreate(samplingTask, "samplingTask", 2048, NULL, 1, &samplingTaskHandle);
    }
}

void Sampler::set(const char* _key, const char* _value) {
    if (String(_key) == String("rawSampleInterval")) {
        config.rawSampleInterval = atol(_value);
    } else if (String(_key) == String("avgSampleInterval")) {
        config.avgSampleInterval = atol(_value);
    } else if (String(_key) == String("avgSamplingStopMode")) {
        memcpy(config.avgSamplingStopMode, _value, strlen(_value) + 1);
    } else if (String(_key) == String("avgSamplingBufferSize")) {
        config.avgSamplingBufferSize = atoi(_value);
    } else if (String(_key) == String("avgSamplingDurationMax")) {
        config.avgSamplingDurationMax = atol(_value);
    } else if (String(_key) == String("totalSamplingBufferSize")) {
        config.totalSamplingBufferSize = atoi(_value);
    } else if (String(_key) == String("totalSamplingStopMode")) {
        memcpy(config.totalSamplingStopMode, _value, strlen(_value) + 1);
    } else if (String(_key) == String("totalSampleDurationMax")) {
        config.totalSampleDurationMax = atol(_value);
    } else {
        _PTF("    | ERROR: \"%s\" is not part of SamplerConfig_t type.\n", _key);
    }
}

void Sampler::stop() {
    _PTN("[Sampler::stop]");

    if (samplingTaskHandle != NULL) {
        eTaskState taskState = eTaskGetState(samplingTaskHandle);
        if (taskState != eTaskState::eDeleted && state == true) {
            state = false;
            samplingTaskHandle = NULL;
        } else {
            _PTN("    | Sampler not running. Do nothing...");
        }
    } else {
        _PTN("    | Sampler not running. Do nothing...");
    }
}

void Sampler::printConfiguration() {
    _PTF("    | ------ CONFIGURATION ------\n");
    _PTF("    | --- RAW SAMPLING\n");
    _PTF("    | rawSampleInterval: %lu us\n", config.rawSampleInterval);
    _PTF("    | --- AVERAGE SAMPLING\n");
    _PTF("    | avgSampleInterval: %lu us\n", config.avgSampleInterval);
    _PTF("    | avgSamplingStopMode: %s\n", config.avgSamplingStopMode);
    _PTF("    | avgSamplingBufferSize: %d\n", config.avgSamplingBufferSize);
    _PTF("    | avgSamplingDurationMax: %lu us\n", config.avgSamplingDurationMax);
    _PTF("    | --- TOTAL SAMPLING\n");
    _PTF("    | totalSamplingStopMode: %s\n", config.totalSamplingStopMode);
    _PTF("    | totalSamplingBufferSize: %d\n", config.totalSamplingBufferSize);
    _PTF("    | totalSampleDurationMax: %lu us\n", config.totalSampleDurationMax);
    _PTF("    | -----------------------------\n");
}

float* Sampler::avgSampling() {
    // initialize variables
    uint16_t adcRead[LOADCELL_AMOUNT];
    bool stopSampling;
    unsigned int rawSampleCount = 0;
    unsigned long start = micros();

    while (1) {
        if (config.avgSamplingStopMode == "buffer") {
            stopSampling = rawSampleCount >= config.avgSamplingBufferSize;  // stop based on buffer size
        } else if (config.avgSamplingStopMode == "duration") {
            stopSampling = micros() - start > config.avgSamplingDurationMax;  // stop based on time
        } else if (config.avgSamplingStopMode == "bufferduration") {
            stopSampling = rawSampleCount >= config.avgSamplingBufferSize || micros() - start > config.avgSamplingDurationMax;  // stop based on buffer size and time
        }

        if (stopSampling) {
            break;
        }

        adcRead[0] += adc1_get_raw(ADC1_CHANNEL_4);
        adcRead[1] += adc1_get_raw(ADC1_CHANNEL_5);
        adcRead[2] += adc1_get_raw(ADC1_CHANNEL_6);
        adcRead[3] += adc1_get_raw(ADC1_CHANNEL_7);

        delayMicroseconds(config.rawSampleInterval);
        rawSampleCount++;
    }

    float* avgSamples = (float*)malloc(sizeof(float) * LOADCELL_AMOUNT);

    for (int i = 0; i < 4; i++) {
        avgSamples[i] = (float)(adcRead[i] / rawSampleCount);
    }

#ifdef SAMPLING_VERBOSE
    _PTF("    |\n");
    _PTF("    | rawSampleCount: %d\n", rawSampleCount);
    _PTF("    | time elapsed: %lu us\n", micros() - start);
#endif  // SAMPLING_VERBOSE

    return avgSamples;
}

void samplingTask(void* pvParameters) {
    _PTN("[samplingTask] Start");
    bool stopSampling;
    unsigned long start = millis();
    unsigned int avgSampleCount = 0;

    // print configuration
    sampler.printConfiguration();
    _PTF("    |\n");
    _PTF("    | ------ SAMPLING: START ------\n");

    // write new sample data file
    // add configuration and timestamp
    // ...

    while (sampler.state) {
        float* avgSamples;
        avgSamples = sampler.avgSampling();

        // append sample data file
        // ...

        avgSampleCount++;

#ifdef SAMPLING_VERBOSE
        _PTF("    | sampling count: %d\n", avgSampleCount);
        for (int i = 0; i < LOADCELL_AMOUNT; i++) {
            _PTF("    | avgSample loadcell[%d]: %.2f\n", i, avgSamples[i]);
        }
#else   // !defined(SAMPLING_VERBOSE)
//
#endif  // SAMPLING_VERBOSE

        if (sampler.config.totalSamplingStopMode == "buffer") {
            stopSampling = (avgSampleCount >= sampler.config.totalSamplingBufferSize);  // stop based on buffer size
        } else if (sampler.config.totalSamplingStopMode == "duration") {
            stopSampling = micros() - start > sampler.config.totalSampleDurationMax;  // stop based on time
        } else if (sampler.config.totalSamplingStopMode == "bufferduration") {
            stopSampling = avgSampleCount >= sampler.config.totalSamplingBufferSize || micros() - start > sampler.config.totalSampleDurationMax;  // stop based on buffer size & time
        }

        if (stopSampling) {
            sampler.state = false;
            samplingTaskHandle = NULL;
        }
        delayMicroseconds(sampler.config.avgSampleInterval);
    }

    _PTF("    | ------ SAMPLING: STOP ------\n");
    _PTF("    | avgSampleCount: %d\n", avgSampleCount);
    _PTF("    | time elapsed: %lu ms\n", (millis() - start));

    _PTN("Sampling process stopped.");

    vTaskDelete(NULL);
}