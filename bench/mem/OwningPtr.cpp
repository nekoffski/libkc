#include <benchmark/benchmark.h>

#include "kc/mem/OwningPtr.hpp"

struct Object {
    int x;
    float y;
};

static void mem_OwningPtr_move(benchmark::State& state) {
    auto ptr = kc::mem::makeOwningPtr<Object>(1, 1.0f);
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i) {
            kc::mem::OwningPtr ptr2 = std::move(ptr);
            benchmark::DoNotOptimize(ptr2);
        }
    }
}

static void mem_UniqPtr_move(benchmark::State& state) {
    auto ptr = std::make_unique<Object>(1, 1.0f);
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i) {
            std::unique_ptr ptr2 = std::move(ptr);
            benchmark::DoNotOptimize(ptr2);
        }
    }
}

static void mem_OwningPtr_createAndDestroy(benchmark::State& state) {
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i) {
            auto ptr = kc::mem::makeOwningPtr<Object>(i * 2, i / 2.0f);
            auto val = ptr->x + ptr->y;
            benchmark::DoNotOptimize(ptr);
            benchmark::DoNotOptimize(val);
        }
    }
}

static void mem_UniqPtr_createAndDestroy(benchmark::State& state) {
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i) {
            auto ptr = std::make_unique<Object>(i * 2, i / 2.0f);
            auto val = ptr->x + ptr->y;
            benchmark::DoNotOptimize(ptr);
            benchmark::DoNotOptimize(val);
        }
    }
}

BENCHMARK(mem_OwningPtr_createAndDestroy)
  ->RangeMultiplier(8)
  ->Range(128, 1024 * 1024);
BENCHMARK(mem_UniqPtr_createAndDestroy)->RangeMultiplier(8)->Range(128, 1024 * 1024);

BENCHMARK(mem_OwningPtr_move)->RangeMultiplier(8)->Range(128, 1024 * 1024);
BENCHMARK(mem_UniqPtr_move)->RangeMultiplier(8)->Range(128, 1024 * 1024);
