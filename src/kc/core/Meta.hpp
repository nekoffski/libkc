#pragma once

#include <concepts>
#include <tuple>
#include <type_traits>

namespace kc::core {

template <typename T, typename U, typename V>
struct is_one_of2
    : std::integral_constant<bool, std::is_same<T, U>::value || std::is_same<T, V>::value> {};

template <typename T, typename U, typename V>
constexpr bool is_one_of2_v = is_one_of2<T, U, V>::value;

template <typename T, typename U, typename V, typename W>
struct is_one_of3
    : std::integral_constant<bool, std::is_same<T, U>::value || std::is_same<T, V>::value ||
                                       std::is_same<T, W>::value> {};

template <typename T, typename U, typename V, typename W>
constexpr bool is_one_of3_v = is_one_of3<T, U, V, W>::value;

template <typename T, typename U>
using enable_for_impl = std::enable_if<std::is_same<T, U>::value, U>;

template <bool Condition, typename U>
using enable_if = typename std::enable_if<Condition, U>::type;

// clang-format off
#define enable_for(_type) typename enable_for_impl<T, _type>::type
// clang-format on

template <typename... Args, std::size_t... Index>
auto any_match_impl(std::tuple<Args...> const& lhs, std::tuple<Args...> const& rhs,
                    std::index_sequence<Index...>) -> bool {
    return (... | (std::get<Index>(lhs) == std::get<Index>(rhs)));
}

template <typename... Args>
auto any_match(std::tuple<Args...> const& lhs, std::tuple<Args...> const& rhs) -> bool {
    return any_match_impl(lhs, rhs, std::index_sequence_for<Args...>{});
}

template <typename T>
using get_iterator_type = typename std::decay_t<T>::iterator;

}  // namespace kc::core