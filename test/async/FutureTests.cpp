#include <gtest/gtest.h>

#include "kc/async/ThreadPool.hpp"
#include "kc/core/Scope.hpp"

using namespace kc::async;
using namespace testing;

struct FutureTests : Test {
    FutureTests() : threadPool(size) {}

    void TearDown() override { threadPool.stop(); }

    ThreadPool threadPool;

    static constexpr int size = 1;
};

TEST_F(FutureTests, givenVectorOfFutures_whenWaiting_shouldUnblock) {
    std::vector<Future<void>> futures;

    REPEAT(5) {
        futures.push_back(threadPool.callAsync([]() -> void { /* do nothing */ }));
    }

    wait(futures);
}
