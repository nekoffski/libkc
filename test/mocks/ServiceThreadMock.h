#pragma once

#include <gmock/gmock.h>

#include "kc/service/ServiceThread.h"

struct ServiceThreadMock : public kc::service::ServiceThread {
    MOCK_METHOD(void, join, (), (override));
};

struct ServiceThreadFactoryMock : public kc::service::ServiceThread::Factory {
    MOCK_METHOD(std::unique_ptr<kc::service::ServiceThread>, create, (kc::service::ServiceBase*),
                (override));
};
