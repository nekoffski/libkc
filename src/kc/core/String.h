#pragma once

#include <string>
#include <vector>

#include <fmt/core.h>
#include <fmt/format.h>

namespace kc::core {

void strip(std::string& str, const char characterToStrip = '\0');

std::vector<std::string> split(const std::string& str, const char delimiter);

enum class ExtractingMode { withExtension, withoutExtension };

std::string extractNameFromPath(const std::string& path, ExtractingMode mode);

}  // namespace kc::core

#define DEFINE_TO_STRING(Type, var)                                            \
    std::string toString(const Type&);                                         \
    inline std::ostream& operator<<(std::ostream& stream, const Type& value) { \
        stream << toString(value);                                             \
        return stream;                                                         \
    }                                                                          \
    template <> struct fmt::formatter<Type> : formatter<std::string> {         \
        inline auto format(const Type& c, format_context& ctx) const           \
          -> format_context::iterator {                                        \
            return formatter<std::string>::format(toString(c), ctx);           \
        }                                                                      \
    };                                                                         \
    inline std::string toString(const Type& var)
