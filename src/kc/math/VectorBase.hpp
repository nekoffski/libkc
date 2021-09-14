#pragma once

#include <cmath>

#include <array>
#include <concepts>
#include <numeric>

#include "kc/core/Log.h"
#include "kc/core/Meta.hpp"
#include "kc/core/Range.hpp"
#include "kc/core/Zip.hpp"

namespace kc::math::detail {

using namespace core;

template <typename T, std::size_t elements>
requires std::is_arithmetic_v<T> class VectorBase {
    using BufferType = std::array<T, elements>;

public:
    explicit VectorBase()
        : m_buffer({ 0 }) {
    }

    explicit VectorBase(T value)
        : m_buffer({ value }) {
    }

    explicit VectorBase(const std::array<T, elements>& buffer)
        : m_buffer(buffer) {
    }

    bool equals(const VectorBase& oth) {
        return oth.m_buffer == m_buffer;
    }

    bool operator==(const VectorBase& oth) {
        return equals(oth);
    }

    bool operator!=(const VectorBase& oth) {
        return not equals(oth);
    }

    VectorBase& operator+=(const VectorBase& oth) {
        for (auto&& [selfElement, otherElement] : zip(m_buffer, oth.m_buffer))
            selfElement += otherElement;
        return *this;
    }

    VectorBase& operator-=(const VectorBase& oth) {
        for (auto&& [selfElement, otherElement] : zip(m_buffer, oth.m_buffer))
            selfElement -= otherElement;
        return *this;
    }

    VectorBase& operator/=(T value) {
        for (auto& selfElement : m_buffer)
            selfElement /= value;

        return *this;
    }

    VectorBase& operator*=(T value) {
        for (auto& selfElement : m_buffer)
            selfElement *= value;
        return *this;
    }

    VectorBase operator+(const VectorBase& oth) {
        VectorBase vector;
        for (auto&& [target, lhs, rhs] : zip(vector.m_buffer, m_buffer, oth.m_buffer))
            target = lhs + rhs;
        return vector;
    }

    VectorBase operator-(const VectorBase& oth) {
        VectorBase vector;
        for (auto&& [target, lhs, rhs] : zip(vector.m_buffer, m_buffer, oth.m_buffer))
            target = lhs - rhs;
        return vector;
    }

    VectorBase operator/(T value) {
        VectorBase vector;
        for (auto&& [target, self] : zip(vector.m_buffer, m_buffer))
            target = self / value;
        return vector;
    }

    VectorBase operator*(T value) {
        VectorBase vector;
        for (auto&& [target, self] : zip(vector.m_buffer, m_buffer))
            target = self * value;
        return vector;
    }

    VectorBase& normalize() {
        return operator/=(length());
    }

    VectorBase normalized() const {
        auto copy = *this;
        return copy / length();
    }

    T& operator[](unsigned int index) {
        ASSERT(index <= elements, "Index out of range");
        return m_buffer[index];
    }

    T operator[](unsigned int index) const {
        ASSERT(index <= elements, "Index out of range");
        return m_buffer[index];
    }

    const std::size_t size() const {
        return elements;
    }

    const float length() const {
        return std::sqrt(lengthSquared());
    }

    const float lengthSquared() const {
        static auto addSquared = [](T sum, T value) -> T {
            return sum + value * value;
        };
        return std::accumulate(m_buffer.begin(), m_buffer.end(), static_cast<T>(0), addSquared);
    }

    BufferType& getBuffer() {
        return m_buffer;
    }

    T* getBufferPointer() {
        return m_buffer.data();
    }

protected:
    BufferType m_buffer;
};

}