#pragma once

#include <concepts>

#include "VectorBase.hpp"

namespace kc::math {

template <typename T>
requires std::is_arithmetic_v<T> struct Vector2 : public detail::VectorBase<T, 2> {
    using detail::VectorBase<T, 2>::VectorBase;

    explicit Vector2(T x, T y)
        : detail::VectorBase<T, 2>::VectorBase({ x, y }) {
    }

    T x() const {
        return this->m_buffer[0];
    }

    T y() const {
        return this->m_buffer[1];
    }
};

using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;

}