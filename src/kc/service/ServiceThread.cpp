#include "ServiceThread.h"

namespace kc::service {

std::unique_ptr<ServiceThread> PthreadServiceThread::Factory::create(ServiceBase* service) {
    return std::make_unique<PthreadServiceThread>(service);
}

PthreadServiceThread::PthreadServiceThread(ServiceBase* service)
    : m_thread([service]() { service->run(); }) {}

void PthreadServiceThread::join() { m_thread.join(); }

}  // namespace kc::service