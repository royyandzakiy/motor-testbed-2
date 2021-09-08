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
    _PRINT("[MqttHandler::callback] ");
    _PRINTLN(topic);
    _PRINT("    | Message: ");
    String messageTemp;

    for (int i = 0; i < length; i++) {
        _PRINT((char)message[i]);
        messageTemp += (char)message[i];
    }
    _PRINTLN();

    mqttHandler.processMqttMessages(topic, messageTemp);
}

bool MqttHandler::publish(char *topic, String message) {
    _PRINTF("[MqttHandler::publish] %s:%s\n", topic, message.c_str());
    return publish(topic, message.c_str());
}

bool MqttHandler::publish(char *topic, char *message) {
    _PRINTF("[MqttHandler::publish] %s:%s\n", topic, message);
    return mqttClient.publish(topic, message);
}

void MqttHandler::processMqttMessages(char *topic, String messageTemp) {
    if (topic == commandsTopic) {
        if (messageTemp == "publish") {
            _PRINTLN("    | Publishing message ");
            // publish all messages
            // code here ...
        }
    }
}

int mqttReconnectCount = 0;
void MqttHandler::reconnect() {
    // Loop until we're reconnected
    while (!mqttClient.connected()) {
        _PRINT("[MqttHandler::reconnect] Attempting MQTT connection...");

        // Attempt to connect
        esp_task_wdt_reset();  // WDT reset
        if (mqttClient.connect(mqttClientName, mqttUser, mqttPass)) {
            _PRINTLN(" connected");
            // Subscribe

            _PRINTF("    | Subscribed to topic: %s\n", commandsTopic);
            mqttClient.subscribe(commandsTopic);
            esp_task_wdt_reset();  // WDT reset
        } else {
            _PRINT(" failed, rc=");
            _PRINT(mqttClient.state());
            _PRINTLN(" try again in 5 seconds");
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
        _PRINT("[publishHeartbeatCount] Count: ");
        _PRINTLN(++count);

        if (!mqttClient.publish(heartbeatTopic, String(count).c_str())) {
            _PRINTF("    | Failed to publish %s:%s\n", heartbeatTopic, String(count).c_str());
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
    //         _PRINTF("[publishMessageQueue] %s:%s, %d left\n", temp.topic, temp.message, mqttHandler.queue.length());

    //         // if (!mqttClient.publish(temp.topic, temp.message)) {
    //         //     _PRINTF("    | Failed to publish %s:%s\n", temp.topic, temp.message);
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
//     _PRINT("[MqttHandler::shift]");
//     if (queue.shift(mqttMessage)) {
//         _PRINT(" successfully shifted");
//     } else {
//         _PRINT(" failed to shift");
//     }
//     _PRINTF(", [%d] %s:%s\n", queue.length(), queue.getTail().topic, queue.getTail().message);
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