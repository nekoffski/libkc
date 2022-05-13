#pragma once

#include <any>

#include "fwd.h"

namespace kc::event {

class EventWrapper {
    friend class EventManager;

   public:
    template <typename Event>
    explicit EventWrapper(const Event& event) : m_event(event), m_isHandled(false) {}

    template <typename Event>
    bool is() const {
        return m_event.type() == typeid(Event);
    }

    template <typename Event>
    Event get() {
        return std::any_cast<Event>(m_event);
    }

    void handled();
    bool isHandled() const;

   private:
    std::any m_event;
    bool m_isHandled;
};

}  // namespace kc::event
