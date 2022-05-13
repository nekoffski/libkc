#include <gtest/gtest.h>

#include "kc/core/Utils.hpp"

struct UtilsTests : ::testing::Test {
    std::vector<int> elements = {1, 2, 3, 4};
};

TEST_F(UtilsTests,
       givenContainerWithDesiredElement_whenQueringForIndexOf_shouldReturnCorrectIndex) {
    EXPECT_EQ(kc::core::indexOf(elements, 4), 3);
}

TEST_F(UtilsTests,
       givenContainerWithoutDesiredElement_whenQueringForIndexOf_shouldReturnDefaultValue) {
    EXPECT_EQ(kc::core::indexOf(elements, 5), -1);
}

TEST_F(
    UtilsTests,
    givenContainerWithoutDesiredElementsAndDefaultValueSpecified_whenQueringForIndexOf_shouldReturnSpecifiedValue) {
    constexpr static int valueOnFail = 1337;

    EXPECT_EQ(kc::core::indexOf(elements, 5, valueOnFail), valueOnFail);
}

TEST_F(UtilsTests, givenVectorWithValue_whenCheckingIfContainsValue_shouldReturnTrue) {
    EXPECT_TRUE(kc::core::contains(elements, 1));
}

TEST_F(UtilsTests, givenVectorWithoutValue_whenCheckingIfContainsValue_shouldReturnFalse) {
    EXPECT_FALSE(kc::core::contains(elements, 1337));
}