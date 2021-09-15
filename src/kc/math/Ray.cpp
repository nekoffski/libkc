#include "Ray.h"

namespace kc::math {

Ray::Ray() = default;

Ray::Ray(const Vector3f& origin, const Vector3f& direction)
    : m_origin(origin)
    , m_direction(direction) {
}

const Vector3f& Ray::getOrigin() const {
    return m_origin;
}

const Vector3f& Ray::getDirection() const {
    return m_direction;
}

Vector3f Ray::at(float t) const {
    return m_origin + t * m_direction;
}

}