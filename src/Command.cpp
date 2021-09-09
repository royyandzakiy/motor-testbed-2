#include "Command.h"

Command command;

bool toListen = true;

// COMMAND LISTENER
void Command::process(const String _input) {
    String _command = _input.substring(0, _input.indexOf(':'));
    if (_command == "rawSampleInterval") {
        uint32_t interval = atol(_input.substring(_input.indexOf(':') + 1, _input.indexOf(';')).c_str());
        _PT("Change rawSampleInterval to ");
        _PT(interval);
        _PTN("s");

        deviceConfig.sampling.rawSampleInterval = interval;
        _PTF("deviceConfig.sampling.rawSampleInterval: %lu", deviceConfig.sampling.rawSampleInterval);
    } else if (_command == "at" || _command == "AT") {
        _PTN("OK");
    } else if (_command == "start") {
        // startsampling
        sampler.start();
    } else if (_command == "stop") {
        // stop sampling
        sampler.stop();

        // dataSaver.toFs();
        // dataSaver.toMqtt();

        // switch option of upload
        // if !(auto upload)
        // prompt user
        // ...

        // xTaskCreate(uploadTask, "uploadTask", 2048, NULL, 1, &uploadTaskHandle);
    } else if (_command == "print") {
        sampler.printConfiguration();
    } else if (_command == "set") {
        const String _key = _input.substring(_input.indexOf(':') + 1, _input.indexOf('='));
        const String _value = _input.substring(_input.indexOf('=') + 1, _input.indexOf(';'));

        _PTF("    | [set] %s = %s\n", _key.c_str(), _value.c_str());

        sampler.set(_key.c_str(), _value.c_str()); 
        // set:rawSampleInterval=15;
        // set:avgSamplingBufferSize=15;
        // set:totalSamplingStopMode=bufferdurationtimestamp;
        // print:
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