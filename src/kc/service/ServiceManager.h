#pragma once

#include <vector>

#include "ServiceBase.h"
#include "ServiceThread.h"
#include "kc/core/Log.h"

namespace kc::service {

class ServiceManager {
   public:
    explicit ServiceManager(ServiceThread::Factory* serviceThreadFactory);

    // clang-format off
    template <typename... T>
    void addServices(T&&... services) {
        (addService(std::forward<T>(services)), ...);
    }
    // clang-format on

    void addService(ServiceBase* service);
    void start();

    int getServicesCount();
    void setupSignals();
    void onSignal();

   private:
    void initServices();
    void startThreads();
    void waitForThreads();
    void shutdownServices();

    ServiceThread::Factory* m_serviceThreadFactory;

    std::vector<ServiceBase*> m_services;
    std::vector<std::unique_ptr<ServiceThread>> m_threads;
};

}  // namespace kc::service