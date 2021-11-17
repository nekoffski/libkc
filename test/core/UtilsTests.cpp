#include "kc/core/Utils.hpp"

#include <gtest/gtest.h>

struct IndexOfTests : ::testing::Test {
    std::vector<int> elements = { 1, 2, 3, 4 };
};

TEST_F(IndexOfTests, givenContainerWithDesiredElement_whenQueringForIndexOf_shouldReturnCorrectIndex) {
    EXPECT_EQ(kc::core::indexOf(elements, 4), 3);
}

TEST_F(IndexOfTests, givenContainerWithoutDesiredElement_whenQueringForIndexOf_shouldReturnDefaultValue) {
    EXPECT_EQ(kc::core::indexOf(elements, 5), -1);
}

TEST_F(IndexOfTests, givenContainerWithoutDesiredElementsAndDefaultValueSpecified_whenQueringForIndexOf_shouldReturnSpecifiedValue) {
    constexpr static int valueOnFail = 1337;

    EXPECT_EQ(kc::core::indexOf(elements, 5, valueOnFail), valueOnFail);
}
