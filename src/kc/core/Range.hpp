#pragma once

#include <concepts>
#include <iterator>
#include <utility>

#include "kc/math/Utils.hpp"

#include <iostream>

namespace kc::core {

namespace detail {

    template <typename T>
    class Range {
        class Iterator : public std::iterator<std::forward_iterator_tag, T> {
        public:
            Iterator(T value, T step)
                : m_value(value)
                , m_step(step) {
            }

            T operator*() const {
                return m_value;
            }

            Iterator& operator++() {
                m_value += m_step;
                return *this;
            }

            Iterator& operator++(int) {
                auto tmp = *this;
                m_value += m_step;
                return tmp;
            }

            friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
                return lhs.m_value >= rhs.m_value;
            };

            friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
                return not(lhs == rhs);
            };

        private:
            T m_value;
            T m_step;
        };

    public:
        Range(T begin, T end, T step = static_cast<T>(1))
            : m_begin(begin, step)
            , m_end(end, step) {
        }

        auto begin() {
            return m_begin;
        }

        auto end() {
            return m_end;
        }

        auto cbegin() const {
            return m_begin;
        }

        auto cend() const {
            return m_end;
        }

    private:
        Iterator m_begin;
        Iterator m_end;
    };
}

template <typename T>
requires std::is_arithmetic_v<T> detail::Range<T> range(T begin, T end, T step = static_cast<T>(1)) {
    return detail::Range<T> { begin, end, step };
}

template <typename T>
requires std::is_arithmetic_v<T> detail::Range<T> range(T end) {
    return range(static_cast<T>(0), end);
}

template <typename T>
detail::Range<std::size_t> indicesOf(const T& container) {
    return detail::Range<std::size_t> { 0ul, container.size() - 1ul };
}

}