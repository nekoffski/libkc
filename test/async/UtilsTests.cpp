#include <gtest/gtest.h>

#include "kc/async/Utils.hpp"

using namespace testing;
using namespace kc;

struct AsyncUtilsTests : Test {
    async::TaskScheduler ts{ 4 };
};

struct AsyncLoopTests : public AsyncUtilsTests, public ::WithParamInterface<int> {};

TEST_P(AsyncLoopTests, givenParalellLoop_shouldExecuteAllIterations) {
    const int taskSize      = GetParam();
    std::atomic_int counter = 0;
    std::vector<int> called(taskSize, 0);  // vector of booleans is weird

    async::loop(ts, taskSize, [&](std::size_t index) {
        called[index] = 1337;
        counter++;
    }).wait();

    EXPECT_EQ(counter, taskSize);

    EXPECT_TRUE(std::ranges::all_of(called, [](int value) -> bool {
        return value > 0;
    }));
}

static const std::vector<int> iterations{
    1, 2, 5, 7, 10, 21, 55, 786, 1000, 1337, 10000, 231515, 11111,
};

INSTANTIATE_TEST_SUITE_P(
  ParallelLoopParametrized, AsyncLoopTests, testing::ValuesIn(iterations)
);