#pragma once

#include "EventWrapper.hpp"

namespace kc::event {

struct EventObserver {
    virtual void onEvent(EventWrapper& eventWrapper) = 0;
};

}  // namespace kc::event
