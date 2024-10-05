#ifndef EVENTBUS_H_
#define EVENTBUS_H_

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <typeindex>
#include <vector>

#include "Event.h"

// Example event: MouseMoveEventExample
class MouseMoveEventExample : public Event {
   public:
    MouseMoveEventExample(int x, int y) : x(x), y(y) {}
    int x, y;
};

// EventBus class
class EventBus {
   private:
    // Using type alias for event handler function
    using EventHandler = std::function<void(std::shared_ptr<Event>)>;

    std::queue<std::shared_ptr<Event>> eventQueue;
    std::map<std::type_index, std::vector<EventHandler>> subscribers;

   public:
    // Method to subscribe to a specific event type using a lambda with `auto`
    // parameter
    template <typename EventType>
    void subscribe(std::function<void(const EventType&)> handler) {
        std::type_index typeIndex(typeid(EventType));
        subscribers[typeIndex].push_back(
            [handler](std::shared_ptr<Event> event) {
                // Automatically cast the event and invoke the handler
                handler(*std::static_pointer_cast<EventType>(event));
            });
    }

    // Variadic template method to post an event by constructing it with given
    // arguments
    template <typename EventType, typename... Args>
    void post(Args&&... args) {
        std::shared_ptr<EventType> event =
            std::make_shared<EventType>(std::forward<Args>(args)...);
        eventQueue.push(event);
    }

    // Method to dispatch/poll events
    void dispatch() {
        while (!eventQueue.empty()) {
            std::shared_ptr<Event> event = eventQueue.front();
            eventQueue.pop();

            std::type_index typeIndex(typeid(*event));

            // Dispatch the event to all subscribers of that event type
            if (subscribers.find(typeIndex) != subscribers.end()) {
                for (auto& handler : subscribers[typeIndex]) {
                    handler(event);
                }
            }
        }
    }
};

#endif  // EVENTBUS_H_