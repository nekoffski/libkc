#include <gtest/gtest.h>

#include "kc/core/Macros.h"
#include "kc/math/Utils.hpp"

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
    auto n                          = glm::normalize(glm::vec3{2.5f, 1.0f, 3.0f});
    static constexpr int iterations = 1000;

    REPEAT(iterations) {
        auto vector = randomUnitHemisphereVec3(n);
        EXPECT_GT(glm::dot(vector, n), 0);
    }
}

TEST(UtilsTests, whenSolvingQuadraticEquationWithoutRootInRealNumbers_shouldReturnNone) {
    EXPECT_FALSE(solveQuadraticEquation(100.0f, 1.0f, 1.0f).has_value());
}

TEST(UtilsTests, whenSolvingQuadraticEquationWithDeltaEqualZero_shouldReturnTwoSameValues) {
    auto roots = solveQuadraticEquation(1.0f, 2.0f, 1.0f);

    EXPECT_TRUE(roots.has_value());

    auto [x0, x1] = roots.value();
    EXPECT_EQ(x0, x1);
}

TEST(UtilsTests, whenSolvingQuadraticEquationWithPositiveDelta_shouldReturnTwoDifferentRoots) {
    auto roots = solveQuadraticEquation(1.0f, -14.0f, 11.0f);

    EXPECT_TRUE(roots.has_value());

    constexpr float delta = 0.001f;

    auto [x0, x1] = roots.value();
    EXPECT_NEAR(x0, 0.8355, delta);
    EXPECT_NEAR(x1, 13.1644, delta);
}