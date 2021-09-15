#include "kc/math/Ray.h"

#include <gtest/gtest.h>

using namespace kc::math;

static const Vector3f origin = { 1.0f, 2.0f, 3.0f };
static const Vector3f direction = { 11.0f, 2.3f, 3.1f };

TEST(RayTests, givenRay_whenGettingOrigin_shouldReturnCorrectValue) {
    EXPECT_EQ(Ray(origin, direction).getOrigin(), origin);
}

TEST(RayTests, givenRay_whenGettingDirection_shouldReturnCorrectValue) {
    EXPECT_EQ(Ray(origin, direction).getDirection(), direction);
}

TEST(RayTests, givenDefaultRay_shouldHaveDefaultValues) {
    EXPECT_EQ(Ray().getDirection().length(), 0);
    EXPECT_EQ(Ray().getDirection().length(), 0);
}