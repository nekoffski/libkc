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
        serviceMock = std::make_shared<ServiceMock>("ServiceMock");
        serviceThreadFactoryMock = std::make_shared<ServiceThreadFactoryMock>();
        serviceThreadMock = std::make_shared<ServiceThreadMock>();

        serviceManager = std::make_shared<ServiceManager>(serviceThreadFactoryMock);
    }

    std::shared_ptr<ServiceMock> serviceMock;
    std::shared_ptr<ServiceThreadFactoryMock> serviceThreadFactoryMock;
    std::shared_ptr<ServiceThreadMock> serviceThreadMock;
    std::shared_ptr<ServiceManager> serviceManager;
};

TEST_F(ServiceManagerTests, givenServiceManagerWithoutServices_whenAskingForCountOfServices_shouldReturnZero) {
    EXPECT_EQ(serviceManager->getServicesCount(), 0);
}

TEST_F(ServiceManagerTests, givenServiceManagerWithServices_whenAskingForCountOfServices_shouldReturnOne) {
    ASSERT_EQ(serviceManager->getServicesCount(), 0);

    serviceManager->addService(serviceMock);
    EXPECT_EQ(serviceManager->getServicesCount(), 1);
}

TEST_F(ServiceManagerTests, givenRegisteredService_whenStartingServiceManager_shouldProcessService) {
    EXPECT_CALL(*serviceMock, onInit()).Times(1);
    EXPECT_CALL(*serviceMock, onShutdown()).Times(1);
    EXPECT_CALL(*serviceThreadFactoryMock, create(_)).Times(1).WillOnce(Return(serviceThreadMock));
    EXPECT_CALL(*serviceThreadMock, join()).Times(1);

    serviceManager->addService(serviceMock);
    serviceManager->start();
}

}