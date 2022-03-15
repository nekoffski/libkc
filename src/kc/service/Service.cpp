#include "Service.h"

#include <thread>

#include "kc/core/Log.h"

namespace kc::service {

Service::Service(const std::string& name) : ServiceBase(name), m_isRunning(true) {}

void Service::run() { call(); }

}  // namespace kc::service