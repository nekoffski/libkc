#include <gtest/gtest.h>

#include <ranges>
#include <unordered_map>

#include "kc/core/Macros.h"
#include "kc/core/Uuid.h"

using namespace kc::core;

TEST(UuidTests, givenTwoGeneratedUuids_shouldBeDifferent) {
    EXPECT_NE(generateUuid(), generateUuid());
}

TEST(UuidTests, givenLotOfUuids_shouldBeUnique) {
    static constexpr int uuidsToGenerate = 1000000;
    std::unordered_map<kc::core::Uuid, int> uuids;

    REPEAT(uuidsToGenerate) { uuids[kc::core::generateUuid()]++; }

    for (auto& uuidOccurences : uuids | std::views::values)
        ASSERT_EQ(uuidOccurences, 1) << "UUID conflict!";
}