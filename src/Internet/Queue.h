#pragma once
#include <Arduino.h>

#define MAX_QUEUE_SIZE 30

template <typename T>
class Queue {
  public:
    Queue();
    bool shift(char *, char *);
    bool shift(T);
    T unshift();
    T getHead();
    T getTail();
    int length();
    
  private:
    T * _queue; // consists of all the messages about to be sent out
    int _length;
};

typedef struct {
  long timestamp;
  uint8_t data;
} loadcell_datapoint_t;

class LoadcellDataQueue: public Queue<loadcell_datapoint_t> {};

// #define MESQUEUE_PRINT_ALL