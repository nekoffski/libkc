#pragma once

#include <concepts>

#include "VectorBase.hpp"

namespace kc::math {

template <typename T>
requires std::is_arithmetic_v<T> struct Vector3 : public detail::VectorBase<T, 3> {
    using detail::VectorBase<T, 3>::VectorBase;

    explicit Vector3(T x, T y, T z)
        : detail::VectorBase<T, 3>::VectorBase({ x, y, z }) {
    }

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

using Vector3f = Vector3<float>;
using Vector3i = Vector3<int>;

}