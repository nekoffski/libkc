#pragma once

#include <vector>

#include "Service.h"
#include "ServiceThread.h"
#include "kc/core/Log.h"

namespace kc::service {

class ServiceManager {
public:
    explicit ServiceManager(std::shared_ptr<ServiceThread::Factory> serviceThreadFactory);

    // clang-format off
    template <typename... T>
    void addServices(T&&... services) {
        (addService(std::forward<T>(services)), ...);
    }
    // clang-format on

    void addService(std::shared_ptr<Service> service);
    void start();

    int getServicesCount();
    void setupSignals();
    void onSignal();

private:
    void initServices();
    void startThreads();
    void waitForThreads();
    void shutdownServices();

    std::shared_ptr<ServiceThread::Factory> m_serviceThreadFactory;

    std::vector<std::shared_ptr<Service>> m_services;
    std::vector<std::shared_ptr<ServiceThread>> m_threads;
};

}