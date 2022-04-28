
#pragma once

#include <glm/glm.hpp>
#include <ostream>
#include <string>

#include "kc/core/Log.h"
#include "kc/core/Macros.h"

namespace glm {

std::string toString(const glm::vec2& vector);
std::string toString(const glm::vec3& vector);
std::string toString(const glm::vec4& vector);

TO_STRING_STREAM(glm::vec2);
TO_STRING_STREAM(glm::vec3);
TO_STRING_STREAM(glm::vec4);

}  // namespace glm