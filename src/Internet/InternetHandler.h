#pragma once

#include <Arduino.h>
#include "credentials.h"
#include "Utilities.h"

class InternetHandler {
    public:
        void connect();
        bool reconnect();
        bool isConnected();
        void wifiSetup();
        void tinygsmSetup();

        bool publish(const String, const String);
};

extern InternetHandler internetHandler;

// ***** WIFI & MQTT CONFIGURATIONS *****
// WIFI & MQTT Credentials
#include "credentials.h"

const char wifiSsid[] = WEBSERVER_WIFI_SSID;
const char wifiPass[] = WEBSERVER_WIFI_PASS;

#include <WiFi.h>
#include <PubSubClient.h>

extern WiFiClient client;