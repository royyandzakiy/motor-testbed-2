#pragma once
#include <Arduino.h>

#define MAX_QUEUE_SIZE 30

typedef struct {
  char * topic;
  char * message;
} mqtt_message_t;

class MessageQueue {
  public:
    MessageQueue();
    bool shift(char *, char *);
    bool shift(mqtt_message_t);
    mqtt_message_t unshift();
    mqtt_message_t getHead();
    mqtt_message_t getTail();
    int length();
    
  private:
    mqtt_message_t * messageQueue; // consists of all the messages about to be sent out
    int queueLength;
};

// #define MESQUEUE_PRINT_ALL