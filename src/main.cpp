#include "Utilities.h"
#include "DeviceConfig.h"
#include "Internet/InternetHandler.h"
#include "Command.h"
#include "Sampler.h"
#include "esp_task_wdt.h"

TaskHandle_t commandTaskHandle;

// DATA SAVER
bool toSave = false, toUpload = false;

void savingTask(void* pvParameters) {
    if (toSave) {
        // get last sampleBuffer
        // save to SD Card
        // ...

        // after finish saving, trigger to get file then upload
        if (deviceConfig.upload == DeviceConfig::UploadConfig_t::AUTOMATIC) {
            // toUpload = true;
            // String sampleFile = dataHandler.get();
            // uploader.invoke(sampleFile);
        }
    }
}

void uploaderTask(void* pvParameters) {
    for (;;) {
        // check if in pipeline there are anything to upload
        if (toUpload) {
        }
    }
}

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
    xTaskCreate(commandTask, "commandTask", 2048, NULL, 2, &commandTaskHandle);

    _PTN("=========== LOADCELL:INIT DONE ===========");
}

void loop() {
    // do nothing
    // commandTask(NULL);
    // samplerTask(NULL);
    // uploaderTask(NULL);

    // command.poll(NULL);
}