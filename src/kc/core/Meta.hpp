#pragma once

#include <concepts>
#include <type_traits>

namespace kc::core {

template <typename T, typename U, typename V>
struct is_one_of2 : std::integral_constant<bool,
                        std::is_same<T, U>::value || std::is_same<T, V>::value> { };

template <typename T, typename U, typename V>
constexpr bool is_one_of2_v = is_one_of2<T, U, V>::value;

template <typename T, typename U>
using enable_for_impl = std::enable_if<std::is_same<T, U>::value, U>;

// clang-format off
#define enable_for(_type) typename enable_for_impl<T, _type>::type
// clang-format on

}