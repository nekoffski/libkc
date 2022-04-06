#pragma once

#include <memory>
#include <mutex>
#include <utility>
#include <vector>

#include "Error.h"
#include "Event.h"
#include "fwd.h"
#include "kc/core/Log.h"

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
            std::string destinations;
            for (auto& [id, categoryMap] : m_eventEmitter->m_eventContainer) {
                m_eventEmitter->pushEvent(m_event, categoryMap);
                destinations += fmt::format("{}, ", id);
            }

            destinations = destinations.substr(0, destinations.size() - 2);
            LOG_TRACE("Event {} emitted to [{}]", m_event->asString(), destinations);

            m_emitted = true;
            return Result{m_event->m_result.get_future()};
        }

        Result to(const std::string& destination) && {
            return std::move(*this).to(std::vector<std::string>{destination});
        }

        Result to(const std::vector<std::string>& destinations) && {
            std::string idents;
            for (auto& destination : destinations) {
                if (not m_eventEmitter->m_eventContainer.contains(destination))
                    throw ListenerNotFound{};

                idents += fmt::format("{}, ", destination);
                m_eventEmitter->pushEvent(m_event, m_eventEmitter->m_eventContainer[destination]);
            }

            idents = idents.substr(0, idents.size() - 2);
            LOG_TRACE("Event {} emitted to [{}]", m_event->asString(), idents);

            m_emitted = true;
            return Result{m_event->m_result.get_future()};
        }

       private:
        EventEmitterHandler(EventEmitter* eventEmitter, std::shared_ptr<Event> event,
                            std::mutex& mutex)
            : m_eventEmitter(eventEmitter), m_event(std::move(event)), m_guard(mutex) {}

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
        return EventEmitterHandler{this, std::make_shared<Ev>(std::forward<Args>(args)...),
                                   m_emitterMutex};
    }

   private:
    void pushEvent(std::shared_ptr<Event> event, CategoryToEventQueue& categoryMap);

    EventContainer& m_eventContainer;
    std::mutex m_emitterMutex;
};
}  // namespace kc::event
