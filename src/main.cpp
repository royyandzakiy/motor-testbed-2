#include "Utilities.h"
#include "DeviceConfig.h"
#include "Internet/InternetHandler.h"
#include "Command.h"
#include "Sampler.h"
#include "PumpHandler.h"
#include "Mcp4725Handler.h"
#include "FlowSensor.h"
#include "esp_task_wdt.h"

TaskHandle_t commandTaskHandle;

void commandTask(void* pvParameters) {
    esp_task_wdt_delete(NULL);
    _PTN("[commandTask] started");

    while(1) {
        command.poll();
    }
    vTaskDelete(NULL);
}

// MAIN
void setup() {
    Serial.begin(115200);
    esp_task_wdt_init(60, false);
    _PTN("=========== LOADCELL:INIT START ===========");

    deviceConfig.init();
    internetHandler.connect();
    mcp4725Handler.init();
    pumpHandler.init();
    sampler.init();
    flowSensor.init();
    attachInterrupt(FLOWSENSOR_PIN, flowtickIsr, RISING);
    
    xTaskCreate(commandTask, "commandTask", 2048, NULL, 2, &commandTaskHandle);

    _PTN("=========== LOADCELL:INIT DONE ===========");
}

void loop() {
    vTaskDelete(NULL); // just delete main loop task
}