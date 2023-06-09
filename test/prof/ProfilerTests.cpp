#include <gtest/gtest.h>

#include <chrono>
#include <thread>
#include <iostream>

#include "kc/prof/Profiler.h"

using namespace std::chrono_literals;

struct ProfilerTests : ::testing::Test {
    kc::prof::Profiler profiler;
};

struct A {
    void foo() {
        PROFILE_FUNCTION();
        std::this_thread::sleep_for(200ms);
    }
};

void foo() {
    PROFILE_FUNCTION();

    std::this_thread::sleep_for(100ms);

    {
        PROFILE_SCOPE(testRegion);
        std::this_thread::sleep_for(10ms);
    }

    A{}.foo();
}

void bar() {
    PROFILE_FUNCTION();
    foo();

    std::this_thread::sleep_for(150ms);

    PROFILE_REGION_BEGIN(loop);

    for (int i = 0; i < 5; ++i) {
        PROFILE_SCOPE(loopStep);
        std::this_thread::sleep_for(1000ms);
    }

    PROFILE_REGION_END();
}

void start() {
    PROFILE_FUNCTION();

    foo();
    bar();
}

TEST_F(ProfilerTests, whenProfilingFunction_shouldGiveCorrectResultAndIndendation) {
    start();
    std::cout << profiler.format() << '\n';
}
