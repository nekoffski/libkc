#include <benchmark/benchmark.h>

#include "kc/core/Range.hpp"

static constexpr int innerIterations = 10000;

static void core_Range_native(benchmark::State& state) {
    for (auto _ : state)
        for (int i = 0; i < innerIterations; ++i)
            benchmark::DoNotOptimize(i);
}

BENCHMARK(core_Range_native)->RangeMultiplier(8)->Range(8, 1024 * 16);

static void core_Range_range(benchmark::State& state) {
    for (auto _ : state)
        for (auto i : kc::core::range(innerIterations))
            benchmark::DoNotOptimize(i);
}

BENCHMARK(core_Range_range)->RangeMultiplier(8)->Range(8, 1024 * 16);
