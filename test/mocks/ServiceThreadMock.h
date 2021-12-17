#pragma once

#include "kc/service/ServiceThread.h"

#include <gmock/gmock.h>

struct ServiceThreadMock : public kc::service::ServiceThread {
    MOCK_METHOD(void, join, (), (override));
};

struct ServiceThreadFactoryMock : public kc::service::ServiceThread::Factory {
    MOCK_METHOD(std::shared_ptr<kc::service::ServiceThread>, create,
        (std::shared_ptr<kc::service::Service>), (override));
};
