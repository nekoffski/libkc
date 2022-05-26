#include <benchmark/benchmark.h>

#include "kc/async/ThreadPool.hpp"

auto iterations = benchmark::CreateRange(8, 1024 * 1024 * 64, 16);

std::vector<long int> threads = {1, 4, 8};

static void async_ThreadPool_nativeLoop(benchmark::State& state) {
    const auto iterations = state.range(0);

    for (auto _ : state)
        for (int i = 0; i < iterations; ++i) benchmark::DoNotOptimize(i);
}

BENCHMARK(async_ThreadPool_nativeLoop)->ArgsProduct({iterations});

static void async_TH_loopParallel(benchmark::State& state) {
    const auto iterations = state.range(0);
    const auto threads    = static_cast<std::size_t>(state.range(1));

    kc::async::ThreadPool th{threads};

    for (auto _ : state) {
        th.loopParallel(iterations, [&](int a, int b) -> void {
            for (int i = a; i < b; ++i) benchmark::DoNotOptimize(i);
        });
    }
}

BENCHMARK(async_TH_loopParallel)->ArgsProduct({iterations, threads});
