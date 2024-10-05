#ifndef WINDOWEVENTS_H_
#define WINDOWEVENTS_H_

#include "Event.h"

// Example event: MouseMoveEventExample
class WindowResizeEvent : public Event {
   public:
    const int width;
    const int height;
    WindowResizeEvent(int width, int height) : width(width), height(height) {}
};

#endif  // WINDOWEVENTS_H_