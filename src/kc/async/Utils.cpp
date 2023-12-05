#include "Utils.hpp"

#include <iostream>

namespace kc::async {

FutureCollection<void> loop(
  TaskScheduler& ts, const std::size_t iterations, LoopWorker<void>&& worker
) {
    const auto workers = ts.getWorkerCount();
    const auto batch   = iterations / workers;
    int remainder      = iterations % workers;

    std::vector<Future<void>> futures;
    futures.reserve(workers);

    int rangeEnd = 0;

    for (int i = 0; i < workers; ++i) {
        const int rangeBegin = rangeEnd;
        rangeEnd             = rangeBegin + batch;

        if (remainder-- > 0) ++rangeEnd;

        futures.push_back(ts.call([worker, rangeBegin, rangeEnd] {
            for (int j = rangeBegin; j < rangeEnd; ++j) worker(j);
        }));
    }

    return FutureCollection<void>(std::move(futures));
}

}  // namespace kc::async
