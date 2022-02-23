#pragma once

#include <tuple>

#include "kc/core/Meta.hpp"

namespace kc::core {

namespace detail {

template <typename T>
class EnumerateRange {
   public:
    using ValueIterator = get_iterator_type<T>;
    using Value = std::tuple<std::size_t, typename std::iterator_traits<ValueIterator>::reference>;

    class Iterator : public std::iterator<std::forward_iterator_tag, Value> {
       public:
        Iterator(std::size_t index, ValueIterator&& valueIterator)
            : m_index(index), m_valueIterator(std::forward<ValueIterator>(valueIterator)) {}

        Value operator*() const { return Value(m_index, *m_valueIterator); }

        Iterator& operator++() {
            m_index++;
            m_valueIterator++;

            return *this;
        }

        Iterator operator++(int) {
            auto copy = *this;
            (*this)++;
            return copy;
        }

        friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
            return lhs.m_index == rhs.m_index || lhs.m_valueIterator == rhs.m_valueIterator;
        }

        friend bool operator!=(const Iterator& lhs, const Iterator& rhs) { return not(lhs == rhs); }

       private:
        std::size_t m_index;
        ValueIterator m_valueIterator;
    };

    explicit EnumerateRange(T& container)
        : m_begin(0, container.begin()), m_end(container.size(), container.end()) {}

    auto begin() { return m_begin; }

    auto end() { return m_end; }

   private:
    Iterator m_begin;
    Iterator m_end;
};
}  // namespace detail

template <typename T>
detail::EnumerateRange<T> enumerate(T& container) {
    return detail::EnumerateRange<T>{container};
}

}  // namespace kc::core