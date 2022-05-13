#pragma once

#include <utility>
#include <vector>

#include "EventObserver.h"
#include "EventWrapper.hpp"
#include "kc/core/ErrorBase.hpp"
#include "kc/core/Singleton.hpp"

DEFINE_ERROR(EventError);

namespace kc::event {

class EventManager : public core::Singleton<EventManager> {
   public:
    void registerObserver(EventObserver* observer);
    void unregisterObserver(EventObserver* observer);

    int getObserversCount() const;

    template <typename Event, typename... Args>
    void emitEvent(Args&&... args) {
        emitEvent(Event{std::forward<Args>(args)...});
    }

    template <typename Event>
    void emitEvent(const Event& event) {
        EventWrapper eventWrapper{event};

        for (auto& observer : m_observers) {
            observer->onEvent(eventWrapper);

            if (eventWrapper.isHandled()) return;
        }
    }

   private:
    std::vector<EventObserver*> m_observers;
};

}  // namespace kc::event
