#include <gtest/gtest.h>

#include "kc/json/JsonConfigLoader.hpp"
#include "mocks/FileSystemMock.hpp"

using namespace kc;
using namespace testing;

struct Config {};

struct JsonConfigLoaderTests : Test {
    void SetUp() override { processConfigFieldsCalled = false; }

    inline static bool processConfigFieldsCalled;
};

struct JsonConfigLoader : public json::JsonConfigLoader<Config> {
    void processFields([[maybe_unused]] const json::Node&) override {
        JsonConfigLoaderTests::processConfigFieldsCalled = true;
    }
};

TEST_F(JsonConfigLoaderTests, givenCorruptedString_whenLoadingFromString_shouldThrow) {
    static const auto corruptedJson = "}not_a_json{";

    EXPECT_THROW(JsonConfigLoader{}.fromString(corruptedJson).load(), json::JsonError);
}

TEST_F(JsonConfigLoaderTests, givenValidString_whenLoadingFromString_shouldNotThrow) {
    static const auto validJson = "{\"hello\": \"world\"}";

    EXPECT_NO_THROW(JsonConfigLoader{}.fromString(validJson).load());
    EXPECT_TRUE(processConfigFieldsCalled);
}