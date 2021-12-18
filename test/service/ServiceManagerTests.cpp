#include "kc/service/ServiceManager.h"

#include <memory>

#include <gtest/gtest.h>

#include "mocks/ServiceMock.h"
#include "mocks/ServiceThreadMock.h"

using namespace kc::service;
using namespace testing;

namespace {

struct ServiceManagerTests : public testing::Test {
    void SetUp() override {
        serviceMock = std::make_unique<ServiceMock>("ServiceMock");
        serviceThreadFactoryMock = std::make_unique<ServiceThreadFactoryMock>();
        serviceThreadMock = std::make_unique<ServiceThreadMock>();

        serviceManager = std::make_unique<ServiceManager>(serviceThreadFactoryMock.get());
    }

    std::unique_ptr<ServiceMock> serviceMock;
    std::unique_ptr<ServiceThreadFactoryMock> serviceThreadFactoryMock;
    std::unique_ptr<ServiceThreadMock> serviceThreadMock;
    std::unique_ptr<ServiceManager> serviceManager;
};

TEST_F(ServiceManagerTests, givenServiceManagerWithoutServices_whenAskingForCountOfServices_shouldReturnZero) {
    EXPECT_EQ(serviceManager->getServicesCount(), 0);
}

TEST_F(ServiceManagerTests, givenServiceManagerWithServices_whenAskingForCountOfServices_shouldReturnOne) {
    ASSERT_EQ(serviceManager->getServicesCount(), 0);

    serviceManager->addService(serviceMock.get());
    EXPECT_EQ(serviceManager->getServicesCount(), 1);
}

TEST_F(ServiceManagerTests, givenRegisteredService_whenStartingServiceManager_shouldProcessService) {
    EXPECT_CALL(*serviceMock, onInit()).Times(1);
    EXPECT_CALL(*serviceMock, onShutdown()).Times(1);

    auto& threadMock = *serviceThreadMock;

    EXPECT_CALL(*serviceThreadFactoryMock, create(_))
        .Times(1)
        .WillOnce(Return(ByMove(std::move(serviceThreadMock))));
    EXPECT_CALL(threadMock, join()).Times(1);

    serviceManager->addService(serviceMock.get());
    serviceManager->start();
}

}