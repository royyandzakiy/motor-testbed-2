#include "Sampler.h"
#include "Utilities.h"
#include "esp_task_wdt.h"
#include "Peripherals/LoadSensor.h"

LoadSensor * loadSensor;

void setup() {
    Serial.begin(115200);
    esp_task_wdt_init(120, false);  // turn off wdt panic, so never restart
    _PTN("=========== LOADCELL:INIT START ===========");

    loadSensor = new LoadSensor("motorTestLoadSensor");
    loadSensor->init();

    loadSensorSampler = new Sampler(loadSensor);
    loadSensorSampler->init();

    _PTN("=========== LOADCELL:INIT DONE ==========="); 

    loadSensorSampler->start();
}

void loop() {
    vTaskDelete(NULL);  // just delete main loop task
}