#pragma once

#include <concepts>

#include "VectorBase.hpp"

namespace kc::math {

template <typename T>
requires std::is_arithmetic_v<T> struct Vector2 : public detail::VectorBase<T, 2> {
    using detail::VectorBase<T, 2>::VectorBase;

    Vector2(T x, T y)
        : detail::VectorBase<T, 2>::VectorBase({ x, y }) {
    }

    Vector2(const detail::VectorBase<T, 2>& oth)
        : detail::VectorBase<T, 2>::VectorBase(oth) {
    }

    Vector2(detail::VectorBase<T, 2>&& oth)
        : detail::VectorBase<T, 2>::VectorBase(std::move(oth)) {
    }

    Vector2& operator=(const detail::VectorBase<T, 2>& oth) {
        this->m_buffer = oth.m_buffer;
        return *this;
    }

    Vector2& operator=(const Vector2& oth) {
        this->m_buffer = oth.m_buffer;
        return *this;
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