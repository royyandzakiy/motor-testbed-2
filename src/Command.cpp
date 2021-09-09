#include "Command.h"

Command command;

bool toListen = true;

// COMMAND LISTENER
void Command::process(const String _command) {
    if (_command.substring(0, _command.indexOf(':')) == "rawSampleInterval") {
        uint32_t interval = atol(_command.substring(_command.indexOf(':') + 1, _command.indexOf(';')).c_str());
        _PT("Change rawSampleInterval to ");
        _PT(interval);
        _PTN("s");

        deviceConfig.sampling.rawSampleInterval = interval;
        _PTF("deviceConfig.sampling.rawSampleInterval: %lu", deviceConfig.sampling.rawSampleInterval);
    } else if (_command.substring(0, _command.indexOf(':')) == "start") {
        _PTN("Sampling process start");
        // startsampling

        xTaskCreate(samplingTask, "samplingTask", 2048, NULL, 1, &samplingTaskHandle);
    } else if (_command.substring(0, _command.indexOf(':')) == "stop") {
        _PTN("Sampling process stopped");
        // stop sampling

        vTaskDelete(samplingTaskHandle);
        // xTaskCreate(dataSaverTask, "dataSaverTask", 2048, NULL, 1, &dataSaverHandle);
        // String avgSamplesStr = dataSaver.sammplesToString(sampler.getAvgSamples());
        // _PTN("    | avgSamplesStr: " + avgSamplesStr);
        // dataSaver.toFs();
        // dataSaver.toMqtt();

        // switch option of upload
        // if !(auto upload)
        // prompt user
        // ...

        // xTaskCreate(uploadTask, "uploadTask", 2048, NULL, 1, &uploadTaskHandle);
    }
}

void Command::poll() {
    if (toListen) {
        if (Serial.available() > 0) {
            String command = Serial.readString();
            process(command);
        }
    }
}