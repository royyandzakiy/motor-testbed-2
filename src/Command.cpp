#include "Command.h"

Command command;

bool toListen = true;

// COMMAND LISTENER
void Command::process(const String _command) {
    if (_command.substring(0, _command.indexOf(':')) == "rawSampleInterval") {
        uint32_t interval = atol(_command.substring(_command.indexOf(':') + 1, _command.indexOf(';')).c_str());
        _P("Change rawSampleInterval to ");
        _P(interval);
        _PN("s");

        deviceConfig.sampling.rawSampleInterval = interval;
        _PF("deviceConfig.sampling.rawSampleInterval: %lu", deviceConfig.sampling.rawSampleInterval);
    }
    else if (_command.substring(0, _command.indexOf(':')) == "start") {
        _PN("Sampling process start");
        // startsampling
    }
    else if (_command.substring(0, _command.indexOf(':')) == "stop") {
        _PN("Sampling process stopped");
        // stop sampling

        // sampler.init();
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