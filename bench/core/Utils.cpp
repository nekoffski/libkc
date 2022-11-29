#include <benchmark/benchmark.h>

#include "kc/core/Utils.hpp"

static std::vector<std::string> createContainer(const int n) {
    std::vector<std::string> words;
    words.reserve(n);

    for (int i = 0; i < n; ++i) words.push_back(std::to_string(i));

    return words;
}

static void core_Join(benchmark::State& state) {
    std::vector<std::string> conjunctions = {",", "+", "_", "-", " "};

    for (auto _ : state) {
        auto words = createContainer(state.range(0));

        for (const auto& conjunction : conjunctions) {
            auto sentence = kc::core::join(words, conjunction);
            benchmark::DoNotOptimize(sentence);
        }
    }
}

BENCHMARK(core_Join)->RangeMultiplier(8)->Range(8, 1024 * 64);