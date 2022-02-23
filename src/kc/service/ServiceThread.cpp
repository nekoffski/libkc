#include "ServiceThread.h"

namespace kc::service {

std::unique_ptr<ServiceThread> PthreadServiceThread::Factory::create(Service* service) {
    return std::make_unique<PthreadServiceThread>(service);
}

PthreadServiceThread::PthreadServiceThread(Service* service)
    : m_thread([service]() { service->run(); }) {}

void PthreadServiceThread::join() { m_thread.join(); }
}  // namespace kc::service