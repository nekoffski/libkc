
#pragma once

#include <glm/glm.hpp>
#include <ostream>
#include <string>

#include "kc/core/Log.h"
#include "kc/core/Macros.h"
#include "kc/core/String.h"

DEFINE_TO_STRING(glm::vec2, vector) {
    return fmt::format("[{}, {}]", vector.x, vector.y);
}

DEFINE_TO_STRING(glm::vec3, vector) {
    return fmt::format("[{}, {}, {}]", vector.x, vector.y, vector.z);
}

DEFINE_TO_STRING(glm::vec4, vector) {
    return fmt::format("[{}, {}, {}, {}]", vector.x, vector.y, vector.z, vector.w);
}
