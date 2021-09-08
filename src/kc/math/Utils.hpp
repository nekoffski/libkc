#pragma once

#include <cmath>

#include "kc/core/Meta.hpp"

namespace kc::math {

template <typename T>
requires core::is_one_of2_v<T, float, double> bool areEqual(T lhs, T rhs) {
    return std::fabs(lhs - rhs) < std::numeric_limits<T>::epsilon();
}

}