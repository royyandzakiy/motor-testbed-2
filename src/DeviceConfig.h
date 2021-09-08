#pragma once
#include <Arduino.h>

#include "SdCardHandler.h"

typedef struct {
    const char* staSsid;
    const char* staPass;
    const char* apSsid;
    const char* apPass;
} WifiConfig_t;

typedef struct {
    const char* server;
    const char* clientName;
    const char* user;
    const char* pass;
} MqttConfig_t;

typedef struct {
    uint32_t rawSampleInterval;
    uint32_t avgSampleInterval;
    uint16_t avgSampleSize;
    uint32_t totalBufferSize;
} SamplingConfig_t;

class DeviceConfig {
   public:
    typedef enum {
        AUTOMATIC = 1,
        MANUAL
    } UploadConfig_t;

    bool init();

    bool save();
    String getSaved();

    WifiConfig_t wifi;
    MqttConfig_t mqtt;
    UploadConfig_t upload;
    SamplingConfig_t sampling;
};

extern DeviceConfig deviceConfig;