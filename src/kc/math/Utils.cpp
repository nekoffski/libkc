#include "Utils.hpp"

#include "kc/core/Macros.h"

namespace kc::math {

glm::vec3 randomVec3(float min, float max) {
    return glm::vec3{random<float>(min, max), random<float>(min, max), random<float>(min, max)};
}

glm::vec3 randomNormalVec3() { return glm::normalize(randomVec3()); }

glm::vec3 randomUnitSphereVec3() {
    LOOP {
        auto vector = randomVec3(-1.0f, 1.0f);

        if (auto length = glm::length(vector); length * length >= 1) continue;

        return vector;
    }
}

glm::vec3 randomUnitHemisphereVec3(const glm::vec3& normal) {
    auto vector = randomUnitSphereVec3();
    return glm::dot(vector, normal) > 0 ? vector : -vector;
}

}  // namespace kc::math