#pragma once

#include <concepts>
#include <tuple>
#include <type_traits>
#include <vector>

#include "kc/core/Meta.hpp"

namespace kc::core {

namespace detail {
template <typename T>
using select_iterator_for = std::conditional_t<std::is_const_v<std::remove_reference_t<T>>,
                                               typename std::decay_t<T>::const_iterator,
                                               typename std::decay_t<T>::iterator>;

template <typename... Containers>
class ZipRange {
   public:
    template <typename... Iterators>
    class Iterator {
        template <typename T>
        using value_type = typename std::iterator_traits<T>::reference;

        using Value = std::tuple<value_type<Iterators>...>;

       public:
        explicit Iterator(Iterators&&... iterators)
            : m_iterators(std::forward<Iterators>(iterators)...) {}

        Value operator*() const {
            return std::apply([](auto&&... args) { return Value(*args...); }, m_iterators);
        }

        Iterator& operator++() {
            std::apply([](auto&... iterators) { (iterators++, ...); }, m_iterators);
            return *this;
        }

        Iterator operator++(int) {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
            return any_match(lhs.m_iterators, rhs.m_iterators);
        };

        friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
            return not(lhs == rhs);
        };

       private:
        std::tuple<Iterators...> m_iterators;
    };

    using iterator_type = Iterator<select_iterator_for<Containers>...>;

    auto begin() {
        static auto createIterators = [](auto&&... args) {
            return iterator_type(std::begin(args)...);
        };
        return std::apply(createIterators, m_containers);
    }

    auto end() {
        static auto createIterators = [](auto&&... args) {
            return iterator_type(std::end(args)...);
        };
        return std::apply(createIterators, m_containers);
    }

    ZipRange(Containers&&... containers) : m_containers(std::forward<Containers>(containers)...) {}

   private:
    std::tuple<Containers...> m_containers;
};

}  // namespace detail

template <typename... Containers>
detail::ZipRange<Containers...> zip(Containers&&... containers) {
    return detail::ZipRange<Containers...>{std::forward<Containers>(containers)...};
}
}  // namespace kc::core