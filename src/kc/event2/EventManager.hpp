#pragma once

#include <functional>
#include <vector>
#include <memory>
#include <typeindex>
#include <unordered_map>

#include "kc/core/Singleton.hpp"
#include "kc/core/Log.h"

#include "EventStorage.hpp"

namespace kc::event2 {

static constexpr int defaultEventPoolSize = 128;

class EventManager : public core::Singleton<EventManager> {
    using GenericHandler = std::function<void(EventStorageBase*)>;

   public:
    explicit EventManager(const int eventPoolSize = defaultEventPoolSize) {
        m_events.reserve(eventPoolSize);
    }

    template <typename T, typename Callback> void on(Callback&& callback) {
        const auto typeIndex = std::type_index(typeid(T));
        ASSERT(not m_handlers.contains(typeIndex), "Handler already registered");
        m_handlers[typeIndex] = [callback](EventStorageBase* event) { callback(event->as<T>()); };
    }

    void dispatch() {
        for (auto& event : m_events) {
            if (const auto handler = m_handlers.find(event->getTypeIndex());
                handler != m_handlers.end()) {
                std::invoke(handler->second, event.get());
            }
        }
        m_events.clear();
    }

    template <typename T, typename... Args> void emit(Args&&... args) {
        m_events.push_back(std::make_unique<EventStorage<T>>(std::forward<Args>(args)...));
    }

   private:
    std::vector<std::unique_ptr<EventStorageBase>> m_events;
    std::unordered_map<std::type_index, GenericHandler> m_handlers;
};

}  // namespace kc::event2
