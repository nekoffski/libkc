#pragma once

#include "Vector3.hpp"

namespace kc::math {

class Ray {
public:
    explicit Ray();
    explicit Ray(const Vector3f& origin, const Vector3f& direction);

private:
};

}