#include "ServiceThread.h"

namespace kc::service {

void PthreadServiceThread::ThreadWorker::operator()() {
    m_service->run();
}

std::shared_ptr<ServiceThread> PthreadServiceThread::Factory::create(std::shared_ptr<Service> service) {
    return std::make_shared<PthreadServiceThread>(service);
}

PthreadServiceThread::PthreadServiceThread(std::shared_ptr<Service> service)
    : m_thread(ThreadWorker { service }) {
}

void PthreadServiceThread::join() {
    m_thread.join();
}
}