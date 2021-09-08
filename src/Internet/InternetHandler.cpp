#include "InternetHandler.h"

#include <esp_task_wdt.h>

InternetHandler internetHandler;

WiFiClient client;

void InternetHandler::connect() {
    wifiSetup();
}

bool InternetHandler::isConnected() {
    return (WiFi.status() == WL_CONNECTED);
}

bool InternetHandler::reconnect() {
    WiFi.disconnect();
    WiFi.reconnect();
    return isConnected();
}

void InternetHandler::wifiSetup() {
    _PRINTLN("[InternetHandler::wifiSetup]");
    delay(10);
    // We start by connecting to a WiFi network
    _PRINT("    | Connecting to ");
    _PRINT(wifiSsid);

    WiFi.begin(wifiSsid, wifiPass);

    long startTime = millis();
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        _PRINT(".");
        if (millis() - startTime > 5000)
            ESP.restart();  // restart if taking too long
    }

    _PRINTLN(" WiFi connected!");
    _PRINT("    | IP address: ");
    _PRINTLN(WiFi.localIP());
}