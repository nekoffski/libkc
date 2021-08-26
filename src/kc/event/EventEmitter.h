#pragma once

#include <memory>
#include <mutex>
#include <utility>
#include <vector>

#include "Error.h"
#include "Event.h"
#include "fwd.h"

namespace kc::event {

class EventEmitter {

public:
    class EventEmitterHandler {
        friend class EventEmitter;

    public:
        ~EventEmitterHandler() noexcept(false) {
            if (not m_emitted && m_exceptionStackSize == std::uncaught_exceptions())
                throw NoTargetSpecified{};
        }

        Result toAll() && {
            for (auto& [_, categoryMap] : m_eventEmitter->m_eventContainer)
                m_eventEmitter->pushEvent(m_event, categoryMap);
            m_emitted = true;
            return Result{ m_event->m_result.get_future() };
        }

        Result to(const std::string& destination) && {
            return std::move(*this).to(std::vector<std::string>{destination});
        }

        Result to(const std::vector<std::string>& destinations) && {
            for (auto& destination : destinations) {
                if (not m_eventEmitter->m_eventContainer.contains(destination))
                    throw ListenerNotFound{};

                m_eventEmitter->pushEvent(m_event, 
                    m_eventEmitter->m_eventContainer[destination]);
            }
            m_emitted = true;
            return Result{ m_event->m_result.get_future() };
        }


    private:
        EventEmitterHandler(EventEmitter* eventEmitter, std::shared_ptr<Event> event, std::mutex& mutex) 
            : m_eventEmitter(eventEmitter), m_event(std::move(event)), m_guard(mutex)
        {
        }


        EventEmitter* m_eventEmitter;
        std::shared_ptr<Event> m_event;
        std::lock_guard<std::mutex> m_guard;

        bool m_emitted = false;
        int m_exceptionStackSize = std::uncaught_exceptions();
    };

    friend class EventEmitterHandler;


    explicit EventEmitter(EventContainer& eventContainer);

    std::shared_ptr<EventEmitter> clone() {
        return std::make_shared<EventEmitter>(m_eventContainer);
    }

    template <EventType Ev, typename... Args>
    EventEmitterHandler emit(Args&&... args) {
        return EventEmitterHandler{this, 
            std::make_shared<Ev>(std::forward<Args>(args)...), m_emitterMutex};
    }

private:
    void pushEvent(std::shared_ptr<Event> event, CategoryToEventQueue& categoryMap);

    EventContainer& m_eventContainer;
    std::mutex m_emitterMutex;
};
}
