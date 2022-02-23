#include "EventEngine.h"

#include <ranges>

#include "Error.h"

namespace kc::event {

void EventEngine::spreadEvents() {
    for (auto& [ident, eventListener] : m_eventListeners)
        eventListener->handleEvents(EventProvider{m_eventContainer[ident]});

    for (auto& categoryEventQueue : m_eventContainer | std::views::values)
        for (auto& eventQueue : categoryEventQueue | std::views::values) eventQueue.clearUnsafe();
}

EventProvider EventEngine::getEventProvider(const std::string& ident) {
    return EventProvider{m_eventContainer[ident]};
}

std::shared_ptr<EventEmitter> EventEngine::createEmitter() {
    return std::make_shared<EventEmitter>(m_eventContainer);
}

void EventEngine::registerEventListener(EventListener* eventListener) {
    const auto& ident = eventListener->getIdent();

    auto element = m_eventListeners.find(ident);

    while (element != m_eventListeners.getBuffer().end()) {
        if (element->second == eventListener) throw ListenerAlreadyRegistered{};
        element = std::next(element);
    }

    m_eventListeners.insert(ident, eventListener);

    if (not m_eventContainer.contains(ident)) m_eventContainer[ident] = CategoryToEventQueue{};
}

void EventEngine::unregisterEventListener(EventListener* eventListener) {
    unregisterEventListener(eventListener->getIdent());
}

void EventEngine::unregisterEventListener(const std::string& ident) {
    if (not m_eventContainer.contains(ident) && not m_eventListeners.contains(ident))
        throw ListenerNotFound{};

    m_eventListeners.erase(ident);
    m_eventContainer.erase(ident);
}

bool EventEngine::isListenerRegistered(const std::string& ident) const {
    return m_eventListeners.contains(ident) && m_eventContainer.contains(ident);
}
}  // namespace kc::event
