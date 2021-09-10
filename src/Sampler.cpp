#include "Sampler.h"

Sampler loadcellSampler;
TaskHandle_t samplingTaskHandle;

#define LOADCELL_AMOUNT 4

#define SAMPLING_PRINT_VERBOSE
#define SAMPLING_PRINT_VISUALIZER

void Sampler::init() {
    _PTN("[Sampler::init]");
    defaultConfiguration();
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

void Sampler::reset() {
    defaultConfiguration();
}

void Sampler::set(const char* _key, const char* _value) {
    if (strcmp(_key, "rawSampleInterval") == 0) {
        config.rawSampleInterval = atol(_value);
    } else if (strcmp(_key, "avgSampleInterval") == 0) {
        config.avgSampleInterval = atol(_value);
    } else if (strcmp(_key, "avgSamplingStopMode") == 0) {
        memcpy(config.avgSamplingStopMode, _value, strlen(_value) + 1);
    } else if (strcmp(_key, "avgSamplingBufferSize") == 0) {
        config.avgSamplingBufferSize = atoi(_value);
    } else if (strcmp(_key, "avgSamplingDurationMax") == 0) {
        config.avgSamplingDurationMax = atol(_value);
    } else if (strcmp(_key, "totalSamplingBufferSize") == 0) {
        config.totalSamplingBufferSize = atoi(_value);
    } else if (strcmp(_key, "totalSamplingStopMode") == 0) {
        memcpy(config.totalSamplingStopMode, _value, strlen(_value) + 1);
    } else if (strcmp(_key, "totalSampleDurationMax") == 0) {
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
    _PTF("    | ------ SAMPLER CONFIGURATION ------\n");
    _PTF("    | \n");
    _PTF("    | RAW SAMPLING\n");
    _PTF("    |     rawSampleInterval: %lu us\n", config.rawSampleInterval);
    _PTF("    | \n");
    _PTF("    | AVERAGE SAMPLING\n");
    _PTF("    |     avgSampleInterval: %lu us\n", config.avgSampleInterval);
    _PTF("    |     avgSamplingStopMode: %s\n", config.avgSamplingStopMode);
    _PTF("    |     avgSamplingBufferSize: %d\n", config.avgSamplingBufferSize);
    _PTF("    |     avgSamplingDurationMax: %lu us\n", config.avgSamplingDurationMax);
    _PTF("    | \n");
    _PTF("    | TOTAL SAMPLING\n");
    _PTF("    |     totalSamplingStopMode: %s\n", config.totalSamplingStopMode);
    _PTF("    |     totalSamplingBufferSize: %d\n", config.totalSamplingBufferSize);
    _PTF("    |     totalSampleDurationMax: %lu us\n", config.totalSampleDurationMax);
    _PTF("    | \n");
    _PTF("    | ---------------------------\n");
}

float* Sampler::avgSampling() {
    // initialize variables
    uint16_t adcRead[LOADCELL_AMOUNT];
    bool stopSampling = false;
    unsigned int rawSampleCount = 0;
    unsigned long start = micros();

    while (!stopSampling) {
        adcRead[0] += adc1_get_raw(ADC1_CHANNEL_0); // 36
        adcRead[1] += adc1_get_raw(ADC1_CHANNEL_3); // 39
        adcRead[2] += adc1_get_raw(ADC1_CHANNEL_6); // 34
        adcRead[3] += adc1_get_raw(ADC1_CHANNEL_7); // 35

        delayMicroseconds(config.rawSampleInterval);
        rawSampleCount++;

        if (strcmp(config.avgSamplingStopMode, "buffer") == 0) {
            stopSampling = rawSampleCount >= config.avgSamplingBufferSize;  // stop based on buffer size
        } else if (strcmp(config.avgSamplingStopMode, "duration") == 0) {
            stopSampling = micros() - start > config.avgSamplingDurationMax;  // stop based on time
        } else if (strcmp(config.avgSamplingStopMode, "bufferduration") == 0) {
            stopSampling = rawSampleCount >= config.avgSamplingBufferSize || micros() - start > config.avgSamplingDurationMax;  // stop based on buffer size and time
        }
    }

    float* avgSamples = (float*)malloc(sizeof(float) * LOADCELL_AMOUNT);

    for (int i = 0; i < 4; i++) {
        avgSamples[i] = (float)(adcRead[i] / rawSampleCount);
    }

#ifdef SAMPLING_PRINT_VERBOSE
    _PTF("    |\n");
    _PTF("    | rawSampleCount: %d\n", rawSampleCount);
    _PTF("    | avgSampling time elapsed: %lu us\n", micros() - start);
#endif  // SAMPLING_PRINT_VERBOSE

    return avgSamples;
}

#include "esp_task_wdt.h"

void samplingTask(void* pvParameters) {
    esp_task_wdt_delete(NULL);

    _PTN("[samplingTask] Start");
    bool stopSampling;
    unsigned long start = micros();
    unsigned int avgSampleCount = 0;

    // print configuration
    loadcellSampler.printConfiguration();
    _PTF("    |\n");
    _PTF("    | ------ SAMPLING: START ------\n");

    // write new sample data file
    // add configuration and timestamp
    // ...

    while (loadcellSampler.state) {
        float* avgSamples;
        avgSamples = loadcellSampler.avgSampling();

        // append sample data file
        // ...

        avgSampleCount++;

#ifdef SAMPLING_PRINT_VERBOSE
        _PTF("    | avgSamplingCount: %d\n", avgSampleCount);
        for (int i = 0; i < LOADCELL_AMOUNT; i++) {
            _PTF("    | avgSample loadcell[%d]: %.2f\n", i, avgSamples[i]);
        }
#elif defined(SAMPLING_PRINT_VISUALIZER)
        // ...
#endif  // SAMPLING_PRINT_VERBOSE

        if (strcmp(loadcellSampler.config.totalSamplingStopMode, "buffer") == 0) {
            stopSampling = (avgSampleCount >= loadcellSampler.config.totalSamplingBufferSize);  // stop based on buffer size
        } else if (strcmp(loadcellSampler.config.totalSamplingStopMode, "duration") == 0) {
            stopSampling = micros() - start > loadcellSampler.config.totalSampleDurationMax;  // stop based on time
        } else if (strcmp(loadcellSampler.config.totalSamplingStopMode, "bufferduration") == 0) {
            stopSampling = avgSampleCount >= loadcellSampler.config.totalSamplingBufferSize || micros() - start > loadcellSampler.config.totalSampleDurationMax;  // stop based on buffer size & time
        }

        if (stopSampling) {
            loadcellSampler.state = false;
            samplingTaskHandle = NULL;
        }
        delayMicroseconds(loadcellSampler.config.avgSampleInterval);
    }

    _PTF("    | ------ SAMPLING: STOP ------\n");
    _PTF("    | avgSampleCount: %d\n", avgSampleCount);
    _PTF("    | total sampling time elapsed: %lu ms\n", (micros() - start)/1000);

    _PTN("Sampling process stopped.");

    vTaskDelete(NULL);
}

void defaultConfiguration() {
    /**
     * --------- SAMPLING CONFIGURATIONS ---------
     */
    // --------- RAW SAMLING ---------
    loadcellSampler.set("rawSampleInterval", "10000");  // interval from each raw sampling (in microseconds). need to create dynamically, count total sample count, reduce by estimated sisa waktu. default is 0

    // --------- AVERAGE SAMPLING ---------
    loadcellSampler.set("avgSampleInterval", "1000000");  // in microseconds
    loadcellSampler.set("avgSamplingStopMode", "buffer");
    loadcellSampler.set("avgSamplingBufferSize", "60");        // is buffer size"
    loadcellSampler.set("avgSamplingDurationMax", "1000000");  // max raw sampling time is 1 second (in microseconds)

    // --------- TOTAL SAMPLING ---------
    loadcellSampler.set("totalSamplingStopMode", "buffer");
    loadcellSampler.set("totalSamplingBufferSize", "100");       // is "buffer size"
    loadcellSampler.set("totalSampleDurationMax", "10000000");  // an alternative to not stop by buffer size, but time (in microseconds)
}
