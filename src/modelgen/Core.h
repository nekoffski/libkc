#pragma once

#include <kc/core/ErrorBase.hpp>

DEFINE_ERROR(ModelGeneratorError);
DEFINE_SUB_ERROR(SyntaxError, ModelGeneratorError);

enum class OutputType {
    singleFile,
    multiFile,
};

static const std::string fourSpaces = "    ";
