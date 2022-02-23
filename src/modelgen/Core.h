#pragma once

#include <kc/core/ErrorBase.hpp>

DEFINE_ERROR(ModelGeneratorError);
DEFINE_SUB_ERROR(SyntaxError, ModelGeneratorError);

enum class OutputType {
    singleFile,
    multiFile,
};

enum class JsonLib { libkc, arduino };

inline std::string spaces(const std::size_t count) {
    static const char space = ' ';
    return std::string(count, space);
}
