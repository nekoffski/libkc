#include "kc/math/Vector2.hpp"
#include "kc/math/Vector3.hpp"
#include "kc/math/Vector4.hpp"

#include <gtest/gtest.h>

using namespace kc;
using namespace kc::math;
using namespace testing;

struct VectorTests : Test {
    inline static constexpr float delta = 0.000001f;
};

TEST_F(VectorTests, givenVector_whenGettingSize_shouldReturnCorrectSize) {
    EXPECT_EQ(Vector2f {}.size(), 2);
    EXPECT_EQ(Vector3f {}.size(), 3);
    EXPECT_EQ(Vector4f {}.size(), 4);
};

TEST_F(VectorTests, givenVector_whenGettingSquaredLength_shouldReturnCorrectLength) {
    EXPECT_EQ(Vector2f(1.5f, 2.5f).lengthSquared(), 1.5f * 1.5f + 2.5f * 2.5f);
    EXPECT_EQ(Vector3f(1.5f, 2.5f, 3.0f).lengthSquared(), 1.5f * 1.5f + 2.5f * 2.5f + 3.0f * 3.0f);
    EXPECT_EQ(Vector4f(1.5f, 2.5f, 3.0f, 4.1f).lengthSquared(), 1.5f * 1.5f + 2.5f * 2.5f + 3.0f * 3.0f + 4.1f * 4.1f);

    EXPECT_EQ(Vector2i(1, 2).lengthSquared(), 1 * 1 + 2 * 2);
    EXPECT_EQ(Vector3i(1, 2, 3).lengthSquared(), 1 * 1 + 2 * 2 + 3 * 3);
    EXPECT_EQ(Vector4i(1, 2, 3, 4).lengthSquared(), 1 * 1 + 2 * 2 + 3 * 3 + 4 * 4);
}

TEST_F(VectorTests, givenVector_whenGettingLength_shouldReturnCorrectLength) {
    EXPECT_NEAR(Vector2f(1.5f, 2.5f).length(), std::sqrt(1.5f * 1.5f + 2.5f * 2.5f), delta);
    EXPECT_NEAR(Vector3f(1.5f, 2.5f, 3.0f).length(), std::sqrt(1.5f * 1.5f + 2.5f * 2.5f + 3.0f * 3.0f), delta);
    EXPECT_NEAR(Vector4f(1.5f, 2.5f, 3.0f, 4.1f).length(), std::sqrt(1.5f * 1.5f + 2.5f * 2.5f + 3.0f * 3.0f + 4.1f * 4.1f), delta);

    EXPECT_NEAR(Vector2i(1, 2).length(), std::sqrt(1 * 1 + 2 * 2), delta);
    EXPECT_NEAR(Vector3i(1, 2, 3).length(), std::sqrt(1 * 1 + 2 * 2 + 3 * 3), delta);
    EXPECT_NEAR(Vector4i(1, 2, 3, 4).length(), std::sqrt(1 * 1 + 2 * 2 + 3 * 3 + 4 * 4), delta);
}

TEST_F(VectorTests, givenEqualVectors_whenCheckingIfAreEqual_shouldReturnTrue) {
    EXPECT_TRUE(Vector2f(1.0f, 1.5f).equals(Vector2f(1.0f, 1.5f)));
    EXPECT_TRUE(Vector3f(1.0f, 1.5f, 1.0f).equals(Vector3f(1.0f, 1.5f, 1.0f)));
    EXPECT_TRUE(Vector4f(1.0f, 1.5f, 1.0f, 0.5f).equals(Vector4f(1.0f, 1.5f, 1.0f, 0.5f)));
}

TEST_F(VectorTests, givenNotEqualVectors_whenCheckingIfAreEqual_shouldReturnTrue) {
    EXPECT_FALSE(Vector2f(1.0f, 1.5f).equals(Vector2f(1.0f, 1.15f)));
    EXPECT_FALSE(Vector3f(1.0f, 1.5f, 1.0f).equals(Vector3f(1.0f, 1.5f, 1.01f)));
    EXPECT_FALSE(Vector4f(1.0f, 1.5f, 1.0f, 0.5f).equals(Vector4f(1.0f, 1.5f, 1.0f, 0.511f)));
}