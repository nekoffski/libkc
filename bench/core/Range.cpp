#include <benchmark/benchmark.h>

#include "kc/core/Range.hpp"

static void core_Range_native(benchmark::State& state) {
    auto innerIterations = (int)state.range(0);
    for (auto _ : state)
        for (int i = 0; i < innerIterations; ++i)
            benchmark::DoNotOptimize(i);
}

BENCHMARK(core_Range_native)->RangeMultiplier(8)->Range(8, 1024 * 16);

static void core_Range_range(benchmark::State& state) {
    auto innerIterations = (int)state.range(0);
    for (auto _ : state)
        for (auto i : kc::core::range(innerIterations))
            benchmark::DoNotOptimize(i);
}

BENCHMARK(core_Range_range)->RangeMultiplier(8)->Range(8, 1024 * 16);

static constexpr int iStep = 3;

static void core_Range_stepIntegerNative(benchmark::State& state) {
    auto innerIterations = (int)state.range(0);
    for (auto _ : state)
        for (int i = 0; i < innerIterations; i += 3)
            benchmark::DoNotOptimize(i);
}

BENCHMARK(core_Range_stepIntegerNative)->RangeMultiplier(8)->Range(8, 1024 * 16);

static void core_Range_stepIntegerRange(benchmark::State& state) {
    auto innerIterations = (int)state.range(0);
    for (auto _ : state)
        for (auto i : kc::core::range(0, innerIterations).withStep(iStep))
            benchmark::DoNotOptimize(i);
}

BENCHMARK(core_Range_stepIntegerRange)->RangeMultiplier(8)->Range(8, 1024 * 16);

static constexpr float fStep = 0.2f;

static void core_Range_stepFloatNative(benchmark::State& state) {
    auto innerIterations = (float)state.range(0);
    for (auto _ : state)
        for (int i = 0; i < innerIterations; i += 3)
            benchmark::DoNotOptimize(i);
}

BENCHMARK(core_Range_stepFloatNative)->RangeMultiplier(2.0f)->Range(1.0f, 10.0f);

static void core_Range_stepFloatRange(benchmark::State& state) {
    auto innerIterations = (float)state.range(0);
    for (auto _ : state)
        for (auto i : kc::core::range(0.0f, innerIterations).withStep(fStep))
            benchmark::DoNotOptimize(i);
}

BENCHMARK(core_Range_stepFloatRange)->RangeMultiplier(2.0f)->Range(1.0f, 10.0f);