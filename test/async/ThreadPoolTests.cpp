#include "kc/async/ThreadPool.hpp"

#include <gtest/gtest.h>

#include "kc/core/Scope.hpp"

using namespace kc::async;
using namespace testing;

struct ThreadPoolTests : Test {
    ThreadPoolTests()
        : threadPool(size) {
    }

    void TearDown() override {
        threadPool.stop();
    }

    ThreadPool threadPool;

    static constexpr int size = 1;
};

TEST(ThreadPoolTests_, givenThreadPool_whenGettingSize_shouldReturnDefaultSize) {
    ThreadPool threadPool;

    ON_SCOPE_EXIT { threadPool.stop(); };

    EXPECT_EQ(threadPool.getSize(), ThreadPool::defaultSize);
}

TEST_F(ThreadPoolTests, givenThreadPool_whenCallingParallelLoop_shouldLoopCorrectTimes) {
    static constexpr int iterations = 1337;

    int expectedSum = 0;
    for (int i = 0; i < iterations; ++i)
        expectedSum += i;

    std::atomic_int receivedSum = 0;

    threadPool.loopParallel(iterations, [&](int index) {
        receivedSum += index;
    });

    EXPECT_EQ(expectedSum, receivedSum);
}

TEST_F(ThreadPoolTests, givenThreadPoolWithCustomSize_whenGettingSize_shouldReturnCorrectSize) {
    EXPECT_EQ(threadPool.getSize(), size);
}

TEST_F(ThreadPoolTests, giveThreadPool_whenExecutingTask_shouldReturnCorrectValue) {
    int value = 5;

    auto task = threadPool.callAsync([&] { return value; });
    wait(task);

    EXPECT_EQ(task.getValue(), value);
}

TEST_F(ThreadPoolTests, giveThreadPool_whenExecutingMultipleTasks_shouldReturnCorrectValue) {
    int value = 5;

    auto task = threadPool.callAsync([&] { return value; });
    auto task1 = threadPool.callAsync([&] { return value + 1; });

    wait(task, task1);

    EXPECT_EQ(task.getValue(), value);
    EXPECT_EQ(task1.getValue(), value + 1);
}
