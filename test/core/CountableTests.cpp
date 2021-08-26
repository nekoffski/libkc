#include "kc/core/Countable.hpp"

#include <gtest/gtest.h>

using namespace kc::core;

struct F : Countable<F> {
};

TEST(CountableTests, givenCountable_whenCreatingAnotherInstances_shouldGiveConsecutiveIds) {
    F f;
    EXPECT_EQ(f.getId(), 0);

    F f2;
    EXPECT_EQ(f2.getId(), 1);

    F f3;
    EXPECT_EQ(f3.getId(), 2);
}

TEST(CountableTests, givenCountable_whenDestroying_shouldKeepFreedId) {
    F f;
    EXPECT_EQ(f.getId(), 0);

    {
        F f1;
        EXPECT_EQ(f1.getId(), 1);
    }

    F f2;
    EXPECT_EQ(f2.getId(), 1);
}
