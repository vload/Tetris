#ifndef KEYBOARDEVENTS_H_
#define KEYBOARDEVENTS_H_

#include "Event.h"

class KeyPressEvent : public Event {
   public:
    const int key_code;
    KeyPressEvent(int key_code) : key_code(key_code) {}
};

class KeyReleaseEvent : public Event {
   public:
    const int key_code;
    KeyReleaseEvent(int key_code) : key_code(key_code) {}
};

#endif  // KEYBOARDEVENTS_H_