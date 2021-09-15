#pragma once

#include <concepts>

#include "VectorBase.hpp"

namespace kc::math {

template <typename T>
requires std::is_arithmetic_v<T> struct Vector3 : public detail::VectorBase<T, 3> {
    using detail::VectorBase<T, 3>::VectorBase;

    Vector3(T x, T y, T z)
        : detail::VectorBase<T, 3>::VectorBase({ x, y, z }) {
    }

    Vector3(const detail::VectorBase<T, 3>& oth)
        : detail::VectorBase<T, 3>::VectorBase(oth) {
    }

    Vector3(detail::VectorBase<T, 3>&& oth)
        : detail::VectorBase<T, 3>::VectorBase(std::move(oth)) {
    }

    friend Vector3 crossProduct<>(const Vector3& lhs, const Vector3& rhs);

    T x() const {
        return this->m_buffer[0];
    }

    T y() const {
        return this->m_buffer[1];
    }

    T z() const {
        return this->m_buffer[2];
    }
};

template <typename T>
Vector3<T> crossProduct(const Vector3<T>& lhs, const Vector3<T>& rhs) {
    return Vector3 {
        lhs.m_buffer[1] * rhs.m_buffer[2] - lhs.m_buffer[2] * rhs.m_buffer[1],
        lhs.m_buffer[2] * rhs.m_buffer[0] - lhs.m_buffer[0] * rhs.m_buffer[2],
        lhs.m_buffer[0] * rhs.m_buffer[1] - lhs.m_buffer[1] * rhs.m_buffer[0]
    };
}

using Vector3f = Vector3<float>;
using Vector3i = Vector3<int>;
}