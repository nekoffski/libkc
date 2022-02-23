#include <gtest/gtest.h>

#include <glm/geometric.hpp>

#include "kc/math/Ray.h"

using namespace kc::math;

static const glm::vec3 origin = {1.0f, 2.0f, 3.0f};
static const glm::vec3 direction = {11.0f, 2.3f, 3.1f};

TEST(RayTests, givenRay_whenGettingOrigin_shouldReturnCorrectValue) {
    EXPECT_EQ(Ray(origin, direction).getOrigin(), origin);
}

TEST(RayTests, givenRay_whenGettingDirection_shouldReturnCorrectValue) {
    EXPECT_EQ(Ray(origin, direction).getDirection(), direction);
}

TEST(RayTests, givenDefaultRay_shouldHaveDefaultValues) {
    EXPECT_EQ(glm::length(Ray().getDirection()), 0);
    EXPECT_EQ(glm::length(Ray().getDirection()), 0);
}