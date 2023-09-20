#include "EventManager.hpp"

namespace kc::event2 {

EventManager::EventManager(const int eventPoolSize) { m_events.reserve(eventPoolSize); }

void EventManager::dispatch() {
    for (auto& event : m_events) {
        if (const auto handler = m_handlers.find(event->getTypeIndex());
            handler != m_handlers.end()) {
            std::invoke(handler->second, event.get());
        }
    }
    m_events.clear();
}

}  // namespace kc::event2
