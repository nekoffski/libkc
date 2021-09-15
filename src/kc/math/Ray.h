#pragma once

#include "Vector3.hpp"

namespace kc::math {

class Ray {
public:
    explicit Ray();
    explicit Ray(const Vector3f& origin, const Vector3f& direction);

    const Vector3f& getOrigin() const;
    const Vector3f& getDirection() const;

    Vector3f at(float t) const;

private:
    Vector3f m_origin;
    Vector3f m_direction;
};

}