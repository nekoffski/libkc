#include "kc/math/Utils.hpp"

#include <gtest/gtest.h>

#include "kc/core/Macros.h"

using namespace kc::math;

TEST(UtilsTests, givenRadiansValue_whenConvertingToDegrees_shouldReturnCorrectValue) {
    EXPECT_EQ(toDegrees(2 * std::numbers::pi), 360);
}

TEST(UtilsTests, givenDegreesValue_whenConvertingToRadians_shouldReturnCorrectValue) {
    EXPECT_NEAR(toRadians(360.0f), 2 * std::numbers::pi, 0.0001f);
}

TEST(UtilsTests, whenGettingRandomFloat_shouldReturnRandomNumbersInRange) {
    float min = 0.0f;
    float max = 5.0f;

    REPEAT(100) {
        auto number = random<float>(min, max);
        EXPECT_LE(number, max);
        EXPECT_GE(number, min);
    }
}

TEST(UtilsTests, whenGettingRandomInt_shouldReturnRandomNumbersInRange) {
    int min = 2;
    int max = 7;

    REPEAT(100) {
        auto number = random<int>(min, max);
        EXPECT_LE(number, max);
        EXPECT_GE(number, min);
    }
}

TEST(UtilsTests, whenGeneratingRandomVectorInHemisphere_shouldBeInHemisphere) {
    auto n = glm::normalize(glm::vec3 { 2.5f, 1.0f, 3.0f });
    static constexpr int iterations = 1000;

    REPEAT(iterations) {
        auto vector = randomUnitHemisphereVec3(n);
        EXPECT_GT(glm::dot(vector, n), 0);
    }
}