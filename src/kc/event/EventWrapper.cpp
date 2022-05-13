#include "EventWrapper.hpp"

namespace kc::event {

void EventWrapper::handled() { m_isHandled = true; }

bool EventWrapper::isHandled() const { return m_isHandled; }

}  // namespace kc::event
