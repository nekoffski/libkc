#pragma once

#include <cmath>
#include <concepts>
#include <glm/glm.hpp>
#include <numbers>
#include <optional>
#include <random>

#include "kc/core/Meta.hpp"

namespace kc::math {

template <typename T>
using infinity = std::numeric_limits<T>::infinity();

template <typename T>
requires std::is_arithmetic_v<T> T toRadians(T degrees) {
    const static T halfFullAngle = static_cast<T>(180);
    return degrees * std::numbers::pi / halfFullAngle;
}

template <typename T>
requires std::is_arithmetic_v<T> T toDegrees(T radians) {
    const static T halfFullAngle = static_cast<T>(180);
    return radians * halfFullAngle / std::numbers::pi;
}

template <typename T>
requires std::is_floating_point_v<T> core::enable_if<std::is_floating_point_v<T>, T> random(
    T min = static_cast<T>(0), T max = static_cast<T>(1)) {
    static std::default_random_engine generator;

    std::uniform_real_distribution<T> distribution(min, max);
    return distribution(generator);
}

template <typename T>
requires std::is_integral_v<T> core::enable_if<std::is_integral_v<T>, T> random(
    T min = static_cast<T>(0), T max = static_cast<T>(1)) {
    static std::default_random_engine generator;

    std::uniform_int_distribution<T> distribution(min, max);
    return distribution(generator);
}

template <typename T>
requires std::is_floating_point_v<T> std::optional<std::pair<T, T>> solveQuadraticEquation(T a, T b,
                                                                                           T c) {
    T delta = b * b - 4 * a * c;
    if (delta < 0) {
        return {};
    } else if (delta == 0) {
        T x = -0.5 * b / a;
        return std::make_pair(x, x);
    }

    T deltaSqrt = std::sqrt(delta);
    T q = (b > 0) ? -0.5 * (b + deltaSqrt) : -0.5 * (b - deltaSqrt);

    T x0 = q / a;
    T x1 = c / q;

    if (x0 > x1) std::swap(x0, x1);

    return std::make_pair(x0, x1);
}

glm::vec3 randomVec3(float min = 0.0f, float max = 1.0f);
glm::vec3 randomNormalVec3();
glm::vec3 randomUnitSphereVec3();
glm::vec3 randomUnitHemisphereVec3(const glm::vec3& normal);

}  // namespace kc::math