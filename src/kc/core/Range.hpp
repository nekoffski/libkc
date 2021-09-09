#pragma once

#include <concepts>
#include <iterator>
#include <utility>

namespace kc::core {

namespace detail {

    template <typename T>
    class Range {
        class Iterator : public std::iterator<std::forward_iterator_tag, T> {
        public:
            Iterator(T value)
                : m_value(value) {
            }

            T operator*() const {
                return m_value;
            }

            Iterator& operator++() {
                m_value++;
                return *this;
            }

            Iterator& operator++(int) {
                auto tmp = *this;
                ++m_value;
                return tmp;
            }

            friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
                return lhs.m_value == rhs.m_value;
            };

            friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
                return not(lhs == rhs);
            };

        private:
            T m_value;
        };

    public:
        Range(T begin, T end)
            : m_begin(begin)
            , m_end(end) {
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
requires std::is_arithmetic_v<T> detail::Range<T> range(T begin, T end) {
    return detail::Range<T> { begin, end };
}

template <typename T>
requires std::is_arithmetic_v<T> detail::Range<T> range(T end) {
    return range(0, end);
}

template <typename T>
detail::Range<std::size_t> indicesOf(const T& container) {
    return detail::Range<std::size_t> { 0ul, container.size() - 1ul };
}

}