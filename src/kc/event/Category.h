#pragma once

#include <concepts>

namespace kc::event {

struct Category {};

struct DefaultCategory : Category {};

template <typename T>
concept CategoryType = std::derived_from<T, Category>;
}  // namespace kc::event
