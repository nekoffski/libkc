#include "kc/core/Range.hpp"

#include <gtest/gtest.h>

using namespace kc;

TEST(RangeTests, giveRangeFromAToB_whenIterating_shouldIterateOverValues) {
    int i = 0;

    for (auto index : core::range(5))
        EXPECT_EQ(index, i++);

    EXPECT_EQ(i, 5);

    int begin = 15;
    int end = 27;

    for (auto index : core::range(begin, end))
        EXPECT_EQ(index, begin++);

    EXPECT_EQ(begin, end);
}

TEST(RangeTests, givenIndices_shouldReturnCorrectRange) {
    std::array<int, 5> arr = { 0 };
    int i = 0;

    for (auto index : core::indicesOf(arr))
        EXPECT_EQ(index, i++);

    EXPECT_EQ(i, arr.size() - 1);
}

TEST(RangeTests, givenIntRangeWithStep_WhenIterating_shouldIterateOverCorrectValues) {
    int value = 0;
    int step = 3;

    for (auto i : core::range(0, 9).withStep(step)) {
        EXPECT_EQ(i, value);

        value += step;
    }

    EXPECT_EQ(value, 9);
}

TEST(RangeTests, givenFloatRangeWithStep_WhenIterating_shouldIterateOverCorrectValues) {
    float value = 0.0f;
    float step = 0.1f;

    for (auto i : core::range(0.0f, 10.0f).withStep(step)) {
        EXPECT_EQ(i, value);

        value += step;
    }

    EXPECT_NEAR(value, 10.0f, 0.0001f);
}