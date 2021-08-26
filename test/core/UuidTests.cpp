#include "kc/core/Uuid.h"

#include <gtest/gtest.h>

using namespace kc::core;

TEST(UuidTests, givenTwoGeneratedUuids_shouldBeDifferent) {
    EXPECT_NE(generateUuid(), generateUuid());
}
