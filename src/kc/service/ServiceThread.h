#pragma once

#include <memory>
#include <thread>

#include "ServiceBase.h"

namespace kc::service {

struct ServiceThread {
    struct Factory {
        virtual std::unique_ptr<ServiceThread> create(ServiceBase* service) = 0;
    };

    virtual ~ServiceThread() = default;

    virtual void join() = 0;
};

class PthreadServiceThread : public ServiceThread {
   public:
    struct Factory : ServiceThread::Factory {
        std::unique_ptr<ServiceThread> create(ServiceBase* service) override;
    };

    explicit PthreadServiceThread(ServiceBase* service);

    void join() override;

   private:
    std::thread m_thread;
};

}  // namespace kc::service