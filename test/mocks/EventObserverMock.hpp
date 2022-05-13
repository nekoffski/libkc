#pragma once

#include <gmock/gmock.h>

#include "kc/event/EventObserver.h"

struct EventObserverMock : kc::event::EventObserver {
    MOCK_METHOD(void, onEvent, (kc::event::EventWrapper &), (override));
};
