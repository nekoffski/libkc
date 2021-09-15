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

TEST_F(VectorTests, givenVector_whenGettingNormalized_shouldReturnNormalizedVector) {
    Vector3f vector { 2.5f, 4.4f, 7.7f };

    auto normalized = vector.normalized();
    EXPECT_NEAR(normalized.length(), 1.0f, 0.0001f);
    EXPECT_NE(vector.length(), 1.0f);
    EXPECT_NE(&vector, &normalized);
}

TEST_F(VectorTests, givenVector_whenNormalizing_shouldReturnNormalizedSelf) {
    Vector3f vector { 2.5f, 4.4f, 7.7f };

    auto& normalized = vector.normalize();

    EXPECT_NEAR(normalized.length(), 1.0f, 0.0001f);
    EXPECT_NEAR(vector.length(), 1.0f, 0.0001f);
    EXPECT_EQ(&vector, &normalized);
}

TEST_F(VectorTests, givenTwoVectors_whenCalculatingDotProduct_shouldReturnCorrectValue) {
    Vector3f a { 1.0f, 2.0f, 3.0f };
    Vector3f b { 1.5f, 2.5f, 3.5f };

    EXPECT_NEAR(dotProduct(a, b), 1.0f * 1.5f + 2.0f * 2.5f + 3.0f * 3.5f, 0.0001f);
}

TEST_F(VectorTests, whenGeneratingRandomVectorInHemisphere_shouldBeInHemisphere) {
    auto n = Vector3f { 2.5f, 1.0f, 3.0f }.normalized();
    static constexpr int iterations = 1000;

    REPEAT(iterations) {
        auto vector = Vector3f::randomInUnitHemisphere(n);
        EXPECT_GT(dotProduct(vector, n), 0);
    }
}

TEST_F(VectorTests, givenReflectedVector_whenCheckingSize_shouldMatch) {
    Vector3f n { 1.0f, 1.0f, 1.0f };
    Vector3f a { 5.0f, 3.0f, 1.0f };

    EXPECT_NEAR(a.length(), reflect(a, n.normalized()).length(), 0.0001f);
}

TEST_F(VectorTests, givenRefractedVector_whenCheckingSize_shouldMatch) {
    Vector3f n { 1.0f, 1.0f, 1.0f };
    Vector3f a { 5.0f, 3.0f, 1.0f };

    float ior = 1.0f;

    EXPECT_NEAR(a.length(), refract(a, n.normalized(), ior).length(), 0.0001f);
}

TEST_F(VectorTests, givenVector_whenGettingNegativeVector_shouldMatchNegativeComponents) {
    Vector3f a { 5.0f, 3.0f, 1.0f };

    Vector3f na = -a;

    EXPECT_NEAR(a[0], -na[0], 0.0001f);
    EXPECT_NEAR(a[1], -na[1], 0.0001f);
    EXPECT_NEAR(a[2], -na[2], 0.0001f);

    EXPECT_NEAR(a.x(), -na.x(), 0.0001f);
    EXPECT_NEAR(a.y(), -na.y(), 0.0001f);
    EXPECT_NEAR(a.z(), -na.z(), 0.0001f);
}