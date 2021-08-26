#include "EventProvider.h"

namespace kc::event {

EventProvider::EventProvider(CategoryToEventQueue& categoryToEventQueue)
    : m_categoryToEventQueue(categoryToEventQueue) {
}

EventQueue EventProvider::getAll() const {
    EventQueue events;
    for (auto& [_, eventQueue] : m_categoryToEventQueue) {
        auto lock = eventQueue.lock();
        events.insert(events.begin(), eventQueue.begin(), eventQueue.end());
        eventQueue.clearUnsafe();
    }

    return events;
}
}
