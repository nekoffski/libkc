#pragma once

#include <concepts>

#include "VectorBase.hpp"

namespace kc::math {

template <typename T>
requires std::is_arithmetic_v<T> struct Vector4 : public detail::VectorBase<T, 4> {
    using detail::VectorBase<T, 4>::VectorBase;

    explicit Vector4(T x, T y, T z, T w)
        : detail::VectorBase<T, 4>::VectorBase({ x, y, z, w }) {
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

    T w() const {
        return this->m_buffer[3];
    }
};

using Vector4f = Vector4<float>;
using Vector4i = Vector4<int>;

}