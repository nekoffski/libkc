#pragma once

#include <memory>
#include <type_traits>

#ifndef SPDLOG_COMPILED_LIB
#error "Please, provide compiled version of libsdplog"
#endif

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/spdlog.h>

namespace kc::core {

void initLogging(const std::string& applicationName);

}

// clang-format off
#define LOG_FATAL(...) SPDLOG_CRITICAL(__VA_ARGS__);
#define LOG_ERROR(...) SPDLOG_ERROR(__VA_ARGS__);
#define LOG_WARN(...) SPDLOG_WARN(__VA_ARGS__)
#define LOG_INFO(...) SPDLOG_INFO(__VA_ARGS__)
#define LOG_TRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define LOG_DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__);
// clang-format on

#define ASSERT(x, ...)                                \
    if (!(x)) {                                       \
        LOG_FATAL(__VA_ARGS__);                       \
        throw std::runtime_error("Assertion failed"); \
    }
