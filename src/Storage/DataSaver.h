#pragma once
#include <Arduino.h>
#include "Utilities.h"
#include "FS.h"
#include "Internet/InternetHandler.h"

class DataSaver {
    public:
        const String sammplesToString(const float *, const int);
        const File sammplesToFile(const float *);
        bool toFs(const String);
        bool toMqtt(const String);
        bool toHttp(const String);

        bool fsMode;
        bool cloudMode;
};

extern void dataSaverTask(void*);
extern TaskHandle_t dataSaverHandle;
extern DataSaver dataSaver;