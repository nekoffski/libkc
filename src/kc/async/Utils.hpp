#pragma once

#include <functional>

#include "TaskScheduler.hpp"

namespace kc::async {

template <typename T> using LoopWorker = std::function<T(std::size_t)>;

FutureCollection<void> loop(
  TaskScheduler& ts, const std::size_t iterations, LoopWorker<void>&& worker
);

}  // namespace kc::async
