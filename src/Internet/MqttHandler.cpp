#include "MqttHandler.h"

#include <esp_task_wdt.h>

MqttHandler mqttHandler;
PubSubClient mqttClient(client);  // client should be defined in InternetHandler

void MqttHandler::setup() {
    mqttClient.setServer(mqttServer, 1883);
    mqttClient.setCallback(callback);

    reconnect();
}

void callback(char *topic, byte *message, unsigned int length) {
    _PT("[MqttHandler::callback] ");
    _PTN(topic);
    _PT("    | Message: ");
    String messageTemp;

    for (int i = 0; i < length; i++) {
        _PT((char)message[i]);
        messageTemp += (char)message[i];
    }
    _PTN();

    mqttHandler.processMqttMessages(topic, messageTemp);
}

bool MqttHandler::publish(char *topic, String message) {
    _PTF("[MqttHandler::publish] %s:%s\n", topic, message.c_str());
    return publish(topic, message.c_str());
}

bool MqttHandler::publish(char *topic, char *message) {
    _PTF("[MqttHandler::publish] %s:%s\n", topic, message);
    return mqttClient.publish(topic, message);
}

void MqttHandler::processMqttMessages(char *topic, String messageTemp) {
    if (topic == commandsTopic) {
        if (messageTemp == "publish") {
            _PTN("    | Publishing message ");
            // publish all messages
            // code here ...
        }
    }
}

int mqttReconnectCount = 0;
void MqttHandler::reconnect() {
    // Loop until we're reconnected
    while (!mqttClient.connected()) {
        _PT("[MqttHandler::reconnect] Attempting MQTT connection...");

        // Attempt to connect
        esp_task_wdt_reset();  // WDT reset
        if (mqttClient.connect(mqttClientName, mqttUser, mqttPass)) {
            _PTN(" connected");
            // Subscribe

            _PTF("    | Subscribed to topic: %s\n", commandsTopic);
            mqttClient.subscribe(commandsTopic);
            esp_task_wdt_reset();  // WDT reset
        } else {
            _PT(" failed, rc=");
            _PT(mqttClient.state());
            _PTN(" try again in 5 seconds");
            mqttReconnectCount++;
            if (mqttReconnectCount > 6) {
                ESP.restart();
            }
            // Wait 5 seconds before retrying
            delay(5000);
        }
        esp_task_wdt_reset();  // WDT reset
    }
}

void MqttHandler::loop(void *pvParameter) {
    uint8_t internetConnectRetry = 0;
    uint8_t mqttConnectRetry = 0;

    while (!mqttClient.connected() && mqttConnectRetry) {
        reconnect();
        delay(1000);
        mqttConnectRetry++;
    }

    esp_task_wdt_reset();  // WDT reset
}

long publishHeartbeatCountLast = 0;
long count = 0;

void publishHeartbeatCount() {
    if (millis() - publishHeartbeatCountLast > 5000) {
        _PT("[publishHeartbeatCount] Count: ");
        _PTN(++count);

        if (!mqttClient.publish(heartbeatTopic, String(count).c_str())) {
            _PTF("    | Failed to publish %s:%s\n", heartbeatTopic, String(count).c_str());
        }
        publishHeartbeatCountLast = millis();
        esp_task_wdt_reset();  // WDT reset
    }
}

long publishMessageQueueLast;

void publishMessageQueue() {
    // while (mqttHandler.queue.length() > 0) {
    //     if (millis() - publishMessageQueueLast > 5000) {
    //         mqtt_message_t  temp = mqttHandler.queue.unshift();
    //         _PTF("[publishMessageQueue] %s:%s, %d left\n", temp.topic, temp.message, mqttHandler.queue.length());

    //         // if (!mqttClient.publish(temp.topic, temp.message)) {
    //         //     _PTF("    | Failed to publish %s:%s\n", temp.topic, temp.message);
    //         //     esp_task_wdt_reset();  // WDT reset
    //         //     mqttHandler.reconnect();
    //         // }
    //         publishMessageQueueLast = millis();
    //         esp_task_wdt_reset();  // WDT reset
    //     }
    //     esp_task_wdt_reset();  // WDT reset
    // }
}

// void MqttHandler::shift(mqtt_message_t mqttMessage) {
//     _PT("[MqttHandler::shift]");
//     if (queue.shift(mqttMessage)) {
//         _PT(" successfully shifted");
//     } else {
//         _PT(" failed to shift");
//     }
//     _PTF(", [%d] %s:%s\n", queue.length(), queue.getTail().topic, queue.getTail().message);
// }

// void MqttHandler::shift(char *topic, char *message) {
//     mqtt_message_t temp = {topic, message};
//     shift(temp);
// }

// mqtt_message_t MqttHandler::unshift() {
//     return queue.unshift();
// }

// int MqttHandler::length() {
//     return queue.length();
// }