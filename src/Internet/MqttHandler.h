#pragma once

#include "InternetHandler.h"
#include "credentials.h"
#include "Utilities.h"

// Add your MQTT Broker IP address, example:
const char mqttServer[] = MQTT_SERVER;
const char mqttUser[] = MQTT_USER;
const char mqttPass[] = MQTT_PASS;
const char mqttClientName[] = MQTT_CLIENT;

const char loadcellTopic[] = "x/loadcell/values";
const char heartbeatTopic[] = "x/loadcell/heartbeat";
const char commandsTopic[] = "x/loadcell/command";

#include <PubSubClient.h>
extern PubSubClient mqttClient;
class MqttHandler {
   public:
    void setup();
    void reconnect();
    void loop(void *pvParameter);
    void processMqttMessages(char *, String);
    bool publish(char *, String);
    bool publish(char *, char *);
};

void callback(char *, byte *, unsigned int);
void publishHeartbeatCount();

extern MqttHandler mqttHandler;