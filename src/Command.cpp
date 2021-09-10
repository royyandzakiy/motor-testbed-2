#include "Command.h"

Command serialCommand;
TaskHandle_t commandTaskHandle;

bool toListen = true;

void commandTask(void* pvParameters) {
    esp_task_wdt_delete(NULL);
    _PTN("[commandTask] started");

    while (1) {
        serialCommand.poll();
    }
    vTaskDelete(NULL);
}

// COMMAND LISTENER
void Command::init() {
    _PTN("[Command::init]");

    xTaskCreate(commandTask, "commandTask", 2048, NULL, 2, &commandTaskHandle);
}

void Command::process(const String _input) {
    // GENERAL
    String _command = _input.substring(0, _input.indexOf(':'));
    if (_command == "at" || _command == "AT") {
        _PTN("OK");
    } else if (_command == "samplingStartDemo") {
        samplingStartDemo();
    }

    // LOADCELL SAMPLER
    else if (_command == "startLs") {
        _PTF("    | [%s]\n", _command.c_str());
        // startsampling
        loadcellSampler.start();
    } else if (_command == "stopLs") {
        _PTF("    | [%s]\n", _command.c_str());
        // stop sampling
        loadcellSampler.stop();
    } else if (_command == "stateLs") {
        _PTF("    | [%s] %s\n", _command.c_str(), loadcellSampler.state ? "running" : "stopped");
    } else if (_command == "printLs") {
        _PTF("    | [%s]\n", _command.c_str());
        loadcellSampler.printConfiguration();
    } else if (_command == "resetLs") {
        _PTF("    | [%s]\n", _command.c_str());
        loadcellSampler.reset();
    } else if (_command == "setLs") {
        _PTF("    | [%s]", _command.c_str());

        const String _key = _input.substring(_input.indexOf(':') + 1, _input.indexOf('='));
        const String _value = _input.substring(_input.indexOf('=') + 1, _input.indexOf(';'));

        _PTF(" %s = %s\n", _key.c_str(), _value.c_str());

        loadcellSampler.set(_key.c_str(), _value.c_str());
    }

    // PUMP OUT
    else if (_command == "startPo") {
        _PTF("    | [%s]\n", _command.c_str());
        pumpOut.start();
    } else if (_command == "stopPo") {
        _PTF("    | [%s]\n", _command.c_str());
        pumpOut.stop();
    } else if (_command == "statePo") {
        _PTF("    | [%s] %s\n", _command.c_str(), pumpOut.state ? "running" : "stopped");
    } else if (_command == "printPo") {
        _PTF("    | [%s]\n", _command.c_str());
        pumpOut.printConfiguration();
    } else if (_command == "setPo") {
        _PTF("    | [%s]", _command.c_str());

        const String _key = _input.substring(_input.indexOf(':') + 1, _input.indexOf('='));
        const String _value = _input.substring(_input.indexOf('=') + 1, _input.indexOf(';'));

        _PTF(" %s = %s\n", _key.c_str(), _value.c_str());

        pumpOut.set(_key.c_str(), _value.c_str());
    }

    // PUMP IN
    else if (_command == "startPi") {
        _PTF("    | [%s]\n", _command.c_str());
        pumpIn.start();
    } else if (_command == "stopPi") {
        _PTF("    | [%s]\n", _command.c_str());
        pumpIn.stop();
    } else if (_command == "statePi") {
        _PTF("    | [%s] %s\n", _command.c_str(), pumpIn.state ? "running" : "stopped");
    } else if (_command == "printPi") {
        _PTF("    | [%s]\n", _command.c_str());

        pumpIn.printConfiguration();
    } else if (_command == "setPi") {
        _PTF("    | [%s]", _command.c_str());

        const String _key = _input.substring(_input.indexOf(':') + 1, _input.indexOf('='));
        const String _value = _input.substring(_input.indexOf('=') + 1, _input.indexOf(';'));

        _PTF(" %s = %s\n", _key.c_str(), _value.c_str());

        pumpIn.set(_key.c_str(), _value.c_str());
    }

    // GENERAL
    else {
        _PTF("    | ERROR: [%s] doesn't exist\n", _command.c_str());
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