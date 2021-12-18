#pragma once

#include <memory>
#include <thread>

#include "Service.h"

namespace kc::service {

struct ServiceThread {
    struct Factory {
        virtual std::unique_ptr<ServiceThread> create(Service* service) = 0;
    };

    virtual ~ServiceThread() = default;

    virtual void join() = 0;
};

class PthreadServiceThread : public ServiceThread {
public:
    struct Factory : ServiceThread::Factory {
        std::unique_ptr<ServiceThread> create(Service* service) override;
    };

    explicit PthreadServiceThread(Service* service);

    void join() override;

private:
    std::thread m_thread;
};

}