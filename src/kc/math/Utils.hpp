#pragma once

#include <cmath>

#include <utility>

#include "kc/core/Meta.hpp"

namespace kc::math {

template <typename T>
bool areEqual(T lhs, T rhs) {
    return std::fabs(lhs - rhs) < std::numeric_limits<T>::epsilon();
}

template <typename T>
bool isGreater(T lhs, T rhs) {
    // TODO: investigate
    return lhs - std::numeric_limits<float>::epsilon() * 1000 > rhs;
}

}