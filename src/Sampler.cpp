#include "Sampler.h"

Sampler sampler;

// SAMPLER
bool toSample = false;
hw_timer_t* samplerTimer = NULL;
portMUX_TYPE samplerMux = portMUX_INITIALIZER_UNLOCKED;

#define RAW_SAMPLE_BUFFER_SIZE 100
#define AVG_SAMPLE_BUFFER_SIZE 5

uint16_t sampleBuffer[RAW_SAMPLE_BUFFER_SIZE];

hw_timer_t* timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

volatile int interruptCounter;
uint32_t totalInterruptCounter = 0;

void IRAM_ATTR onTimer() {
    portENTER_CRITICAL_ISR(&timerMux);
    interruptCounter++;
    portEXIT_CRITICAL_ISR(&timerMux);
}

void IRAM_ATTR sampleActivate() {
    portENTER_CRITICAL_ISR(&samplerMux);
    toSample = true;
    portEXIT_CRITICAL_ISR(&samplerMux);
}

#define SAMPLING_INTERVAL_MILLIS 1000 // 1000000 micro seconds

void Sampler::init() {
    long intervalMillis = SAMPLING_INTERVAL_MILLIS;
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, intervalMillis, true);
    timerAlarmEnable(timer);
}

void Sampler::samplerTask(void* pvParameters) {
    if (toSample) {
        Serial.print("An interrupt as occurred.");

        portENTER_CRITICAL(&samplerMux);
        toSample = false;
        portEXIT_CRITICAL(&samplerMux);

        // sampler logic here
        // ...

        // savingTask start
        // xTaskCreate(savingTask, "savingTask", 2048, NULL, 1, NULL);
    }
}

void Sampler::poll() {
    if (interruptCounter > 0) {
        if (totalInterruptCounter % 100 == 0) {
            Serial.print("An interrupt as occurred. Total number: ");
            Serial.print(totalInterruptCounter);
            Serial.print("; Interrupt Counter: ");
            Serial.println(interruptCounter);
        }
        portENTER_CRITICAL(&timerMux);
        interruptCounter--;
        portEXIT_CRITICAL(&timerMux);

        if (totalInterruptCounter < 2000000)
            totalInterruptCounter++;
        else
            totalInterruptCounter = 0;
    }
}