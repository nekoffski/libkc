#pragma once

#include <cmath>
#include <concepts>
#include <numbers>
#include <random>

#include "kc/core/Meta.hpp"

namespace kc::math {

template <typename T>
using infinity = std::numeric_limits<T>::infinity();

template <typename T>
requires std::is_arithmetic_v<T> T degreesToRadians(T degrees) {
    const static T halfFullAngle = static_cast<T>(180);
    return degrees * std::numbers::pi / halfFullAngle;
}

template <typename T>
requires std::is_arithmetic_v<T> T radiansToDegrees(T radians) {
    const static T halfFullAngle = static_cast<T>(180);
    return radians * halfFullAngle / std::numbers::pi;
}

// clang-format off
template <typename T>
requires std::is_floating_point_v<T> 
core::enable_if<std::is_floating_point_v<T>, T> random(T min = static_cast<T>(0), T max = static_cast<T>(1)) {
    static std::default_random_engine generator;
    
    std::uniform_real_distribution<T> distribution(min, max);
    return distribution(generator);
}

template <typename T>
requires std::is_integral_v<T> 
core::enable_if<std::is_integral_v<T>, T> random(T min = static_cast<T>(0), T max = static_cast<T>(1)) {
    static std::default_random_engine generator;

    std::uniform_int_distribution<T> distribution(min, max);
    return distribution(generator);
}

// clang-format on

}