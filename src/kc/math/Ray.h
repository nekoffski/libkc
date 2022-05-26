#pragma once

#include <glm/vec3.hpp>

namespace kc::math {

class Ray {
   public:
    explicit Ray(
        const glm::vec3& origin    = glm::vec3{0.0f, 0.0f, 0.0f},
        const glm::vec3& direction = glm::vec3{0.0f, 0.0f, 0.0f}
    );

    const glm::vec3& getOrigin() const;
    const glm::vec3& getDirection() const;

    glm::vec3 at(float t) const;

   private:
    glm::vec3 m_origin;
    glm::vec3 m_direction;
};

}  // namespace kc::math