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
    explicit EventManager(const int eventPoolSize = defaultEventPoolSize);

    template <typename T, typename Callback> EventManager& on(Callback&& callback) {
        const auto typeIndex = std::type_index(typeid(T));
        ASSERT(not m_handlers.contains(typeIndex), "Handler already registered");
        m_handlers[typeIndex] = [callback](EventStorageBase* event) { callback(event->as<T>()); };

        return *this;
    }

    void dispatch();

    template <typename T, typename... Args> void emit(Args&&... args) {
        m_events.push_back(std::make_unique<EventStorage<T>>(std::forward<Args>(args)...));
    }

   private:
    std::vector<std::unique_ptr<EventStorageBase>> m_events;
    std::unordered_map<std::type_index, GenericHandler> m_handlers;
};

}  // namespace kc::event2
