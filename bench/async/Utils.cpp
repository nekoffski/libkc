#include <benchmark/benchmark.h>

#include <cmath>

#include "kc/async/Utils.hpp"

long doSomeWork(int j) {
    long result = 0;
    for (int i = 0; i < 1024 * 8; ++i)
        result += (i * i + std::pow(i, 3) / (std::sqrt(i))) + j;
    return result;
}

static void loopSync(benchmark::State& state) {
    for (auto _ : state) {
        const int iterations = state.range(0);
        for (int i = 0; i < iterations; ++i) benchmark::DoNotOptimize(doSomeWork(i));
    }
}

static void loopParallel(benchmark::State& state) {
    kc::async::TaskScheduler ts;

    for (auto _ : state) {
        const int iterations = state.range(0);
        kc::async::loop(ts, iterations, [](std::size_t i) {
            benchmark::DoNotOptimize(doSomeWork(i));
        }).wait();
    }
}

// BENCHMARK(loopSync)
//   ->RangeMultiplier(8)
//   ->Range(8, 1024 * 64)
//   ->Unit(benchmark::kSecond);
BENCHMARK(loopParallel)
  ->RangeMultiplier(8)
  ->Range(8, 1024 * 64)
  ->Unit(benchmark::kSecond);