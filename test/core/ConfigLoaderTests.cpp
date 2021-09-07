#include "kc/core/ConfigLoader.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mocks/FileSystemMock.hpp"

using namespace kc;
using namespace testing;

struct ConfigLoaderTests : Test {
    void SetUp() override {
        loadConfigCalled = false;
    }

    inline static bool loadConfigCalled;
};

struct Config {
};

struct ConfigLoader : public core::ConfigLoader<Config> {
private:
    void loadConfig([[maybe_unused]] const std::string&) override {
        ConfigLoaderTests::loadConfigCalled = true;
    }
};

TEST_F(ConfigLoaderTests, givenConfigLoader_whenCallingFromFileAndFromString_shouldThrow) {
    EXPECT_THROW(ConfigLoader {}.fromFile("ff").fromString("ff"), core::ErrorBase);
    EXPECT_THROW(ConfigLoader {}.fromString("ff").fromFile("ff"), core::ErrorBase);
}

TEST_F(ConfigLoaderTests, givenConfigLoader_whenLoadingWithoutSource_shouldThrow) {
    EXPECT_THROW(ConfigLoader {}.load(), core::ErrorBase);
}

TEST_F(ConfigLoaderTests, givenConfigLoader_whenLoadingFromFile_shouldReadFileAndCallLoadConfig) {
    core::FileSystemMock fsMock;
    static const auto fileName = "fileName";

    EXPECT_CALL(fsMock, isFile(StrEq(fileName))).Times(1).WillOnce(Return(true));
    EXPECT_CALL(fsMock, readFile(StrEq(fileName))).Times(1).WillOnce(Return(""));

    ConfigLoader {}.fromFile(fileName, fsMock).load();
    EXPECT_TRUE(loadConfigCalled);
}

TEST_F(ConfigLoaderTests, givenConfigLoader_whenLoadingStringe_shouldCallLoadConfig) {
    ConfigLoader {}.fromString("Content").load();
    EXPECT_TRUE(loadConfigCalled);
}

TEST_F(ConfigLoaderTests, givenConfigLoader_whenTryingToLoadFromFileThatNotExist_shouldThrow) {
    core::FileSystemMock fsMock;
    static const auto fileName = "fileName";

    EXPECT_CALL(fsMock, isFile(StrEq(fileName))).Times(1).WillOnce(Return(false));

    EXPECT_THROW(ConfigLoader {}.fromFile(fileName, fsMock).load(), core::ErrorBase);
}