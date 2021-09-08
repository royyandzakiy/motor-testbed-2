#pragma once
#include <Arduino.h>

#define PRINT_MODE_SERIAL

#ifdef PRINT_MODE_SERIAL
#define _PRINT(...) Serial.print(__VA_ARGS__)
#define _PRINTLN(...) Serial.println(__VA_ARGS__)
#define _PRINTF(...) Serial.printf(__VA_ARGS__)
#define _P(...) _PRINT(__VA_ARGS__)
#define _PN(...) _PRINTLN(__VA_ARGS__)
#define _PF(...) _PRINTF(__VA_ARGS__)
#elif defined(PRINT_MODE_WEB)
#define _PRINT(...)            \
    Serial.print(__VA_ARGS__); \
    WebSerial.print(__VA_ARGS__)
#define _PRINTLN(...)            \
    Serial.println(__VA_ARGS__); \
    WebSerial.println(__VA_ARGS__)
#define _PRINTF(...)            \
    Serial.printf(__VA_ARGS__); \
    WebSerial.printf(__VA_ARGS__)
#endif

void restartIf(const bool, const long);
bool retryUntilSuccess(void (*func)(), const int retries);