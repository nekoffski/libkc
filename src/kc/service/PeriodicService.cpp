#include "PeriodicService.h"

#include <chrono>
#include <thread>

#include "kc/core/Log.h"

namespace kc::service {

PeriodicService::PeriodicService(const std::string& name) : ServiceBase(name), m_isRunning(true) {}

void PeriodicService::run() {
    using namespace std::chrono_literals;

    while (m_isRunning) {
        call();
        std::this_thread::sleep_for(50ms);
    }
}

void PeriodicService::stop() {
    LOG_INFO("Forcing service {} to stop.", getName());
    m_isRunning = false;
}

}  // namespace kc::service