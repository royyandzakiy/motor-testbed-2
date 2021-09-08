#include "Queue.h"

template <typename T>
Queue<T>::Queue() {
    _queue = (T *) malloc (sizeof(T) * MAX_QUEUE_SIZE);
}

template <typename T>
int Queue<T>::length() {
    return _length;
}

/**
 * shift: adding a message (String) to the end of the _queue
 */
template <typename T>
bool Queue<T>::shift(T _message) {
    if (length < MAX_QUEUE_SIZE) {
        ++length;
    } else {
        // if more than max size, just drop message
        Serial.println("ERROR:length already MAX, _queue fail to add/shift");
        return false;
    }

    _queue[length - 1] = _message;
#ifdef MESQUEUE_PRINT_ALL
    Serial.println("shift: _queue[" + String(length - 1) + "]: " + _queue[length - 1]);
#endif
    return true;
}

template <typename T>
bool Queue<T>::shift(char * topic, char * message) {
  T _mqttMessage = {topic, message};
  shift(_mqttMessage);
}

/**
 * unshift: remove the first message (String) from the _queue, and shift all messages forward. return out the first message (String)
 */
template <typename T>
T Queue<T>::unshift() {
    if (length > 0) {
        T _message = getHead();  // get first message in _queue

        // shift all messages 1 time
        for (int i = length - 1; i > 0; i--) {
            _queue[i - 1] = _queue[i];
        }
        _queue[length] = NULL;  // delete last message in _queue
        length--;

#ifdef MESQUEUE_PRINT_ALL
        Serial.println("unshift: _message: " + _message);
#endif
        return _message;
    } else {
        return NULL;
    }
}

/**
 * getHead: get the first message (String) at the start of the _queue
 */
template <typename T>
T Queue<T>::getHead() {
    if (length > 0) {
        return _queue[0];
    } else {
        return NULL;
    }
}

/**
 * getTail: get the last message (String) at the end of the _queue
 */
template <typename T>
T Queue<T>::getTail() {
    if (length > 0) {
        return _queue[length - 1];
    } else {
        return NULL;
    }
}