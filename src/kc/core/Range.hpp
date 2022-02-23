#pragma once

#include <concepts>
#include <iostream>
#include <iterator>
#include <utility>

#include "kc/math/Utils.hpp"

namespace kc::core {

namespace detail {
template <typename T>
class IteratorBase : public std::iterator<std::forward_iterator_tag, T> {
   public:
    IteratorBase(T value, T step) : m_value(value), m_step(step) {}

    T operator*() const { return m_value; }

    IteratorBase& operator++() {
        m_value += m_step;
        return *this;
    }

    IteratorBase& operator++(int) {
        auto tmp = *this;
        m_value += m_step;
        return tmp;
    }

   protected:
    T m_value;
    T m_step;
};

template <typename T, typename Iterator>
class RangeBase {
   public:
    RangeBase(const Iterator& begin, const Iterator& end) : m_begin(begin), m_end(end) {}

    auto begin() { return m_begin; }

    auto end() { return m_end; }

    auto cbegin() const { return m_begin; }

    auto cend() const { return m_end; }

   protected:
    Iterator m_begin;
    Iterator m_end;
};

template <typename T>
struct Iterator : IteratorBase<T> {
    using IteratorBase<T>::IteratorBase;

    friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
        return lhs.m_value == rhs.m_value;
    };

    friend bool operator!=(const Iterator& lhs, const Iterator& rhs) { return not(lhs == rhs); };
};

template <typename T>
struct StepIterator : IteratorBase<T> {
    using IteratorBase<T>::IteratorBase;

    friend bool operator==(const StepIterator& lhs, const StepIterator& rhs) {
        return lhs.m_value >= rhs.m_value;
    };

    friend bool operator!=(const StepIterator& lhs, const StepIterator& rhs) {
        return not(lhs == rhs);
    };
};

template <typename T>
class StepRange : public RangeBase<T, StepIterator<T>> {
   public:
    StepRange(T begin, T end, T step)
        : RangeBase<T, StepIterator<T>>::RangeBase(StepIterator<T>{begin, step},
                                                   StepIterator<T>{end, step}) {}
};

template <typename T>
class Range : public RangeBase<T, Iterator<T>> {
   public:
    Range(T begin, T end)
        : RangeBase<T, Iterator<T>>::RangeBase(Iterator<T>{begin, static_cast<T>(1)},
                                               Iterator<T>{end, static_cast<T>(1)}) {}

    StepRange<T> withStep(T step) const { return StepRange<T>{*this->m_begin, *this->m_end, step}; }
};

}  // namespace detail

template <typename T>
requires std::is_arithmetic_v<T> detail::Range<T> range(T begin, T end) {
    return detail::Range<T>{begin, end};
}

template <typename T>
requires std::is_arithmetic_v<T> detail::Range<T> range(T end) {
    return range(static_cast<T>(0), end);
}

template <typename T>
detail::Range<std::size_t> indicesOf(const T& container) {
    return detail::Range<std::size_t>{0ul, container.size() - 1ul};
}

}  // namespace kc::core