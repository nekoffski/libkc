#include "Log.h"

namespace kc::core {

void initLogging(const std::string& applicationName) {
    // clang-format off
    #ifndef SPDLOG_COMPILED_LIB
    #error "spdlog is attached as header-only, please provide compiled version");
    #endif
    // clang-format on

    const auto pattern = "%^[%d-%m-%Y %T] [Th: %t] %-7l [" + applicationName + "]: %v - [%s:%#]%$";

    spdlog::set_pattern(pattern);
    spdlog::set_level(spdlog::level::trace);
}
}  // namespace kc::core