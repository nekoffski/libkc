#pragma once

#include <string>

#include "kc/core/Uuid.h"

namespace kc::prof {

struct Label {
    std::string name;
    float value;
    int indent;
    int order;

    core::Uuid id = core::generateUuid();
};

}  // namespace kc::prof