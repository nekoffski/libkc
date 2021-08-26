#include "Log.h"

namespace kc::core {

void initLogging(const std::string& applicationName) {
    auto pattern = "%^[%d-%m-%Y %T] [Th: %t] %-7l [" + applicationName + "]: %v - [%s:%#]%$";

    spdlog::set_pattern(pattern);
    spdlog::set_level(spdlog::level::trace);
}
}