#include "EventEngine.h"

#include <ranges>

#include "Error.h"
#include "kc/core/Log.h"

namespace kc::event {

void EventEngine::spreadEvents() {
    auto eventContainer = m_eventContainer;

    for (auto& categoryEventQueue : m_eventContainer | std::views::values)
        for (auto& eventQueue : categoryEventQueue | std::views::values) eventQueue.clearUnsafe();

    for (auto& [ident, eventListener] : m_eventListeners)
        eventListener->handleEvents(EventProvider{eventContainer[ident]});
}

EventProvider EventEngine::getEventProvider(const std::string& ident) {
    return EventProvider{m_eventContainer[ident]};
}

std::shared_ptr<EventEmitter> EventEngine::createEmitter() {
    return std::make_shared<EventEmitter>(m_eventContainer);
}

void EventEngine::registerEventListener(EventListener* eventListener) {
    const auto& ident = eventListener->getIdent();

    auto end = m_eventListeners.getBuffer().end();

    for (auto elementsChain = m_eventListeners.find(ident); elementsChain != end;
         elementsChain = std::next(elementsChain))
        if (elementsChain->second == eventListener) throw ListenerAlreadyRegistered{};

    m_eventListeners.insert(ident, eventListener);

    if (not m_eventContainer.contains(ident)) m_eventContainer[ident] = CategoryToEventQueue{};
}

void EventEngine::unregisterEventListener(EventListener* eventListener) {
    auto ident = eventListener->getIdent();
    if (not m_eventContainer.contains(ident) && not m_eventListeners.contains(ident)) return;

    auto elementsChain = m_eventListeners.find(ident);
    auto end = m_eventListeners.getBuffer().end();

    for (; elementsChain != end; elementsChain = std::next(elementsChain)) {
        if (elementsChain->second == eventListener) {
            m_eventListeners.getBuffer().erase(elementsChain);

            if (not m_eventListeners.contains(ident)) m_eventContainer.erase(ident);

            return;
        }
    }
}

bool EventEngine::isListenerRegistered(const std::string& ident) const {
    return m_eventListeners.contains(ident) && m_eventContainer.contains(ident);
}
}  // namespace kc::event
