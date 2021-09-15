#pragma once

#include <cmath>

#include <array>
#include <concepts>
#include <numeric>
#include <ranges>

#include "kc/core/Log.h"
#include "kc/core/Macros.h"
#include "kc/core/Meta.hpp"
#include "kc/core/Range.hpp"
#include "kc/core/Zip.hpp"
#include "kc/math/Utils.hpp"

namespace kc::math {

namespace detail {

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

        VectorBase(const VectorBase& oth) = default;
        VectorBase(VectorBase&& oth) = default;

        bool equals(const VectorBase& oth) const {
            return oth.m_buffer == m_buffer;
        }

        bool operator==(const VectorBase& oth) const {
            return equals(oth);
        }

        bool operator!=(const VectorBase& oth) const {
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

        VectorBase operator+(const VectorBase& oth) const {
            VectorBase vector;
            for (auto&& [target, lhs, rhs] : zip(vector.m_buffer, m_buffer, oth.m_buffer))
                target = lhs + rhs;
            return vector;
        }

        VectorBase operator-(const VectorBase& oth) const {
            VectorBase vector;
            for (auto&& [target, lhs, rhs] : zip(vector.m_buffer, m_buffer, oth.m_buffer))
                target = lhs - rhs;
            return vector;
        }

        VectorBase operator/(T value) const {
            VectorBase vector;
            for (auto&& [target, self] : zip(vector.m_buffer, m_buffer))
                target = self / value;
            return vector;
        }

        VectorBase operator*(T value) const {
            VectorBase vector;
            for (auto&& [target, self] : zip(vector.m_buffer, m_buffer))
                target = self * value;
            return vector;
        }

        VectorBase operator-() const {
            auto copy = *this;
            auto& buffer = copy.m_buffer;
            std::transform(buffer.begin(), buffer.end(), buffer.begin(), [](const T& value) { return -value; });
            return copy;
        }

        friend VectorBase operator*(T value, const VectorBase& rhs) {
            VectorBase vector;
            for (auto&& [target, self] : zip(vector.m_buffer, rhs.m_buffer))
                target = self * value;
            return vector;
        }

        VectorBase& normalize() {
            return operator/=(length());
        }

        VectorBase normalized() const {
            return *this / length();
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

        friend T dotProduct<>(const VectorBase& lhs, const VectorBase& rhs);

        friend VectorBase reflect<>(const VectorBase& vector, const VectorBase& n);
        friend VectorBase refract<>(const VectorBase& vector, const detail::VectorBase<T, elements>& n, float indexOfRefraction);

        static VectorBase random() {
            return random(static_cast<T>(0), static_cast<T>(1));
        }

        static VectorBase random(T min, T max) {
            VectorBase output;
            auto& buffer = output.m_buffer;
            std::generate(buffer.begin(), buffer.end(), [&]() -> T { return math::random<T>(min, max); });
            return output;
        }

        static VectorBase randomNormal() {
            return random().normalized();
        }

        static VectorBase randomInUnitSphere() {
            LOOP {
                auto vector = VectorBase::random(static_cast<T>(-1), static_cast<T>(1));
                if (vector.lengthSquared() >= 1)
                    continue;
                return vector;
            }
        }

        static VectorBase randomInUnitHemisphere(const VectorBase& n);

    protected:
        BufferType m_buffer;
    };
}

template <typename T, std::size_t elements>
T dotProduct(const detail::VectorBase<T, elements>& lhs, const detail::VectorBase<T, elements>& rhs) {
    T output = static_cast<T>(0);
    for (const auto [lhsElements, rhsElement] : core::zip(lhs.m_buffer, rhs.m_buffer))
        output += lhsElements * rhsElement;
    return output;
}

template <typename T, std::size_t elements>
detail::VectorBase<T, elements> reflect(const detail::VectorBase<T, elements>& vector, const detail::VectorBase<T, elements>& n) {
    return vector - 2 * dotProduct(vector, n) * n;
}

template <typename T, std::size_t elements>
detail::VectorBase<T, elements> refract(const detail::VectorBase<T, elements>& vector, const detail::VectorBase<T, elements>& n, float indexOfRefraction) {
    float nDotV = dotProduct(n, vector);
    float k = 1.f - indexOfRefraction * indexOfRefraction * (1.f - nDotV * nDotV);

    if (k < 0.f)
        return detail::VectorBase<T, elements> {};

    return indexOfRefraction * vector - (indexOfRefraction * nDotV + sqrtf(k)) * n;
}

template <typename T, std::size_t elements>
detail::VectorBase<T, elements> detail::VectorBase<T, elements>::randomInUnitHemisphere(const VectorBase& n) {
    auto vector = detail::VectorBase<T, elements>::randomInUnitSphere();
    return dotProduct(vector, n) > 0 ? vector : -vector;
}

}