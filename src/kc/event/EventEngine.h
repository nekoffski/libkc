#pragma once

#include <memory>
#include <queue>
#include <unordered_map>

#include "Error.h"
#include "Event.h"
#include "EventEmitter.h"
#include "EventListener.h"
#include "EventProvider.h"
#include "fwd.h"

namespace kc::event {

class EventEngine {
   public:
    std::shared_ptr<EventEmitter> createEmitter();

    void spreadEvents();
    void registerEventListener(EventListener* eventListener);
    void unregisterEventListener(EventListener* eventListener);
    EventProvider getEventProvider(const std::string& ident);

    bool isListenerRegistered(const std::string& ident) const;

   private:
    EventListeners m_eventListeners;
    EventContainer m_eventContainer;
};
}  // namespace kc::event
