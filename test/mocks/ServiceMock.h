#pragma once

#include "kc/service/Service.h"

#include <gmock/gmock.h>

struct ServiceMock : public kc::service::Service {
    using kc::service::Service::Service;

    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(void, onInit, (), (override));
    MOCK_METHOD(void, onShutdown, (), (override));
};