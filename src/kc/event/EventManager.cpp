#include "EventManager.hpp"

#include "kc/core/Log.h"
#include "kc/core/Utils.hpp"

namespace kc::event {

void EventManager::registerObserver(EventObserver* observer) {
    ASSERT(not core::contains(m_observers, observer), "Observer is already registered");
    m_observers.push_back(observer);
}

void EventManager::unregisterObserver(EventObserver* observer) {
    std::erase(m_observers, observer);
}

int EventManager::getObserversCount() const { return m_observers.size(); }

}  // namespace kc::event
