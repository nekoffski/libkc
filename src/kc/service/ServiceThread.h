#pragma once

#include <memory>
#include <thread>

#include "Service.h"

namespace kc::service {

struct ServiceThread {
    struct Factory {
        virtual std::shared_ptr<ServiceThread> create(std::shared_ptr<Service> service) = 0;
    };

    virtual void join() = 0;
};

class PthreadServiceThread : public ServiceThread {
    struct ThreadWorker {
        void operator()();

        std::shared_ptr<Service> m_service;
    };

public:
    struct Factory : ServiceThread::Factory {
        std::shared_ptr<ServiceThread> create(std::shared_ptr<Service> service) override;
    };

    explicit PthreadServiceThread(std::shared_ptr<Service> service);

    void join() override;

private:
    std::thread m_thread;
};

}