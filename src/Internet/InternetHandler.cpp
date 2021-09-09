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
    _PTN("[InternetHandler::wifiSetup]");
    delay(10);
    // We start by connecting to a WiFi network
    _PT("    | Connecting to ");
    _PT(wifiSsid);

    WiFi.begin(wifiSsid, wifiPass);

    long startTime = millis();
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        _PT(".");
        if (millis() - startTime > 5000)
            ESP.restart();  // restart if taking too long
    }

    _PTN(" WiFi connected!");
    _PT("    | IP address: ");
    _PTN(WiFi.localIP());
}