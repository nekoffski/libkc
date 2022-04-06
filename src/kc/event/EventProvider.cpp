#include "EventProvider.h"

namespace kc::event {

EventProvider::EventProvider(const CategoryToEventQueue& categoryToEventQueue)
    : m_categoryToEventQueue(categoryToEventQueue) {}

EventQueue EventProvider::getAll() const {
    EventQueue events;
    for (const auto& [_, eventQueue] : m_categoryToEventQueue)
        events.insert(events.begin(), eventQueue.cbegin(), eventQueue.cend());

    if (auto n = events.size(); n > 0) LOG_TRACE("Events: {}", n);

    return events;
}
}  // namespace kc::event
