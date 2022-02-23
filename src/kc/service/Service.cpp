#include "Service.h"

#include "kc/core/Log.h"

namespace kc::service {

Service::Service(const std::string& name) : m_name(name), m_isRunning(true) {}

void Service::run() {
    using namespace std::chrono_literals;

    while (m_isRunning) {
        update();
        std::this_thread::sleep_for(50ms);
    }
}

void Service::stop() {
    LOG_INFO("Forcing service {} to stop.", m_name);
    m_isRunning = false;
}

std::string Service::getName() const { return m_name; }
}  // namespace kc::service