#include <Arduino.h>

typedef struct {
  long timestamp;
  uint16_t value;
} data_message_t;

class DataHandler {
    public:
        void setup();

};