#include <benchmark/benchmark.h>

#include "kc/core/Enumerate.hpp"

static auto setupContainer(int size) { return std::vector<int>(size, 1); }

static void core_Enumerate_native(benchmark::State& state) {
    auto c = setupContainer(state.range(0));
    for (auto _ : state) {
        for (int i = 0; i < c.size(); ++i) {
            benchmark::DoNotOptimize(i);
            benchmark::DoNotOptimize(c[i]);
        }
    }
}

BENCHMARK(core_Enumerate_native)->RangeMultiplier(8)->Range(8, 1024 * 16);

static void core_Enumerate_enumerate(benchmark::State& state) {
    auto c = setupContainer(state.range(0));
    for (auto _ : state) {
        for (auto&& [index, value] : kc::core::enumerate(c)) {
            benchmark::DoNotOptimize(index);
            benchmark::DoNotOptimize(value);
        }
    }
}

BENCHMARK(core_Enumerate_enumerate)->RangeMultiplier(8)->Range(8, 1024 * 16);
