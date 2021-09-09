#pragma once
#include <Arduino.h>

#define PRINT_MODE_SERIAL

#ifdef PRINT_MODE_SERIAL
#define _PT(...) Serial.print(__VA_ARGS__)
#define _PTN(...) Serial.println(__VA_ARGS__)
#define _PTF(...) Serial.printf(__VA_ARGS__)
#elif defined(PRINT_MODE_WEB)
#define _PT(...)            \
    Serial.print(__VA_ARGS__); \
    WebSerial.print(__VA_ARGS__)
#define _PTN(...)            \
    Serial.println(__VA_ARGS__); \
    WebSerial.println(__VA_ARGS__)
#define _PTF(...)            \
    Serial.printf(__VA_ARGS__); \
    WebSerial.printf(__VA_ARGS__)
#endif

void restartIf(const bool, const long);
bool retryUntilSuccess(void (*func)(), const int retries);