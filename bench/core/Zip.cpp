#include <benchmark/benchmark.h>

#include <tuple>
#include <vector>

#include "kc/core/Zip.hpp"

static auto setupContainers() {
    static constexpr int size = 10000;

    std::vector<int> c1(size, 1);
    std::vector<float> c2(size, 5.0f);

    return std::make_pair(c1, c2);
}

static void core_Zip_native(benchmark::State& state) {
    auto [c1, c2] = setupContainers();

    for (auto _ : state) {
        for (int i = 0; i < c1.size(); ++i) {
            benchmark::DoNotOptimize(c1[i]);
            benchmark::DoNotOptimize(c2[i]);
        }
    }
}

BENCHMARK(core_Zip_native)->RangeMultiplier(8)->Range(8, 1024 * 16);

static void core_Zip_zip(benchmark::State& state) {
    auto [c1, c2] = setupContainers();

    for (auto _ : state) {
        for (auto&& [a, b] : kc::core::zip(c1, c2)) {
            benchmark::DoNotOptimize(a);
            benchmark::DoNotOptimize(b);
        }
    }
}

BENCHMARK(core_Zip_zip)->RangeMultiplier(8)->Range(8, 1024 * 16);

static void core_Zip_native4Containers(benchmark::State& state) {
    auto [c1, c2] = setupContainers();
    auto [c3, c4] = setupContainers();

    for (auto _ : state) {
        for (int i = 0; i < c1.size(); ++i) {
            benchmark::DoNotOptimize(c1[i]);
            benchmark::DoNotOptimize(c2[i]);
            benchmark::DoNotOptimize(c3[i]);
            benchmark::DoNotOptimize(c4[i]);
        }
    }
}

BENCHMARK(core_Zip_native4Containers)->RangeMultiplier(8)->Range(8, 1024 * 16);

static void core_Zip_zip4Containers(benchmark::State& state) {
    auto [c1, c2] = setupContainers();
    auto [c3, c4] = setupContainers();

    for (auto _ : state) {
        for (auto&& [a, b, c, d] : kc::core::zip(c1, c2, c3, c4)) {
            benchmark::DoNotOptimize(a);
            benchmark::DoNotOptimize(b);
            benchmark::DoNotOptimize(c);
            benchmark::DoNotOptimize(d);
        }
    }
}

BENCHMARK(core_Zip_zip4Containers)->RangeMultiplier(8)->Range(8, 1024 * 16);
