#pragma once

#include <concepts>
#include <tuple>
#include <type_traits>
#include <vector>

#include "kc/core/Meta.hpp"

namespace kc::core {

namespace detail {

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
                : m_iterators(std::forward<Iterators>(iterators)...) {
            }

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

        auto begin() {
            static auto createIterators = [](auto&&... args) {
                return Iterator<get_iterator_type<Containers>...> { std::begin(args)... };
            };
            return std::apply(createIterators, m_containers);
        }

        auto end() {
            static auto createIterators = [](auto&&... args) {
                return Iterator<get_iterator_type<Containers>...> { std::end(args)... };
            };
            return std::apply(createIterators, m_containers);
        }

        ZipRange(Containers&&... containers)
            : m_containers(std::forward<Containers>(containers)...) {
        }

    private:
        std::tuple<Containers...> m_containers;
    };

}

template <typename... Containers>
detail::ZipRange<Containers...> zip(Containers&&... containers) {
    return detail::ZipRange<Containers...> { std::forward<Containers>(containers)... };
}
}