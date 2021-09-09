#include "kc/core/Enumerate.hpp"

#include <gtest/gtest.h>

using namespace kc;

TEST(EnumerateTests, givenEnumerate_whenIterating_shouldReturnCorrectIndicesAndValues) {
    std::array<int, 5> arr = { 1, 5, 7, 3, 2 };
    auto arr2 = arr;
    int idx = 0;

    for (auto&& [index, value] : core::enumerate(arr)) {
        EXPECT_EQ(arr[index], value);
        ++idx;
    }

    EXPECT_EQ(idx, arr.size());

    for (auto&& [index, value] : core::enumerate(arr2))
        value += index;

    for (int i = 0; i < arr.size(); ++i)
        EXPECT_EQ(arr2[i], arr[i] + i);
}