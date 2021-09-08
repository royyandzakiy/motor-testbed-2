#include "MessageQueue.h"

mqtt_message_t emptyMqttMessage = {"DEFAULT_EMPTY_TOPIC", "DEFAULT_EMPTY_MESSAGE"};

MessageQueue::MessageQueue() {
    messageQueue = (mqtt_message_t * ) malloc (sizeof(mqtt_message_t) * MAX_QUEUE_SIZE);
}

int MessageQueue::length() {
    return queueLength;
}

/**
 * shift: adding a message (String) to the end of the messageQueue
 */
bool MessageQueue::shift(mqtt_message_t _message) {
    if (queueLength < MAX_QUEUE_SIZE) {
        ++queueLength;
    } else {
        // if more than max size, just drop message
        Serial.println("ERROR:queueLength already MAX, messageQueue fail to add/shift");
        return false;
    }

    messageQueue[queueLength - 1] = _message;
#ifdef MESQUEUE_PRINT_ALL
    Serial.println("shift: messageQueue[" + String(queueLength - 1) + "]: " + messageQueue[queueLength - 1]);
#endif
    Serial.printf("[MessageQueue::shift] [%d] %s:%s\n", queueLength, messageQueue[queueLength - 1].topic, messageQueue[queueLength - 1].message);
    return true;
}

bool MessageQueue::shift(char * topic, char * message) {
  mqtt_message_t _mqttMessage = {topic, message};
  shift(_mqttMessage);
}

/**
 * unshift: remove the first message (String) from the messageQueue, and shift all messages forward. return out the first message (String)
 */
mqtt_message_t MessageQueue::unshift() {
    if (queueLength > 0) {
        mqtt_message_t _message = getHead();  // get first message in queue

        // shift all messages 1 time
        for (int i = queueLength - 1; i > 0; i--) {
            messageQueue[i - 1] = messageQueue[i];
        }
        messageQueue[queueLength] = emptyMqttMessage;  // delete last message in queue
        queueLength--;

#ifdef MESQUEUE_PRINT_ALL
        Serial.println("unshift: _message: " + _message);
#endif
        Serial.printf("[MessageQueue::unshift] [%d] %s:%s\n", queueLength, _message.topic, _message.message);

        return _message;
    } else {
        return emptyMqttMessage;
    }
}

/**
 * getHead: get the first message (String) at the start of the messageQueue
 */
mqtt_message_t MessageQueue::getHead() {
    if (queueLength > 0) {
        return messageQueue[0];
    } else {
        return emptyMqttMessage;
    }
}

/**
 * getTail: get the last message (String) at the end of the messageQueue
 */
mqtt_message_t MessageQueue::getTail() {
    if (queueLength > 0) {
        return messageQueue[queueLength - 1];
    } else {
        return emptyMqttMessage;
    }
}