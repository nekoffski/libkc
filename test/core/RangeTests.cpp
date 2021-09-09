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
