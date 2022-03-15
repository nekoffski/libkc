#pragma once

#include <gmock/gmock.h>

#include "kc/service/Service.h"

struct ServiceMock : public kc::service::Service {
    using kc::service::Service::Service;

    MOCK_METHOD(void, call, (), (override));
    MOCK_METHOD(void, onInit, (), (override));
    MOCK_METHOD(void, onShutdown, (), (override));
};