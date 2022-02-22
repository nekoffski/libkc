#include "EventEngine.h"
#include "Error.h"

namespace kc::event {

void EventEngine::spreadEvents() {
    for (auto& [ident, eventListener] : m_eventListeners) {
        auto eventProvider = EventProvider { m_eventContainer[ident] };
        eventListener->handleEvents(eventProvider);
    }
}

EventProvider EventEngine::getEventProvider(const std::string& ident) {
    return EventProvider { m_eventContainer[ident] };
}

std::shared_ptr<EventEmitter> EventEngine::createEmitter() {
    return std::make_shared<EventEmitter>(m_eventContainer);
}

void EventEngine::registerEventListener(EventListener* eventListener) {
    const auto& ident = eventListener->getIdent();

    m_eventListeners.insert(ident, eventListener);
    m_eventContainer[ident] = CategoryToEventQueue {};
}

void EventEngine::unregisterEventListener(EventListener* eventListener) {
    unregisterEventListener(eventListener->getIdent());
}

void EventEngine::unregisterEventListener(const std::string& ident) {
    if (not m_eventContainer.contains(ident) && not m_eventListeners.contains(ident))
        throw ListenerNotFound {};

    m_eventListeners.erase(ident);
    m_eventContainer.erase(ident);
}

bool EventEngine::isListenerRegistered(const std::string& ident) const {
    return m_eventListeners.contains(ident) && m_eventContainer.contains(ident);
}
}
