#include "kc/core/String.h"

#include <numeric>

#include <gtest/gtest.h>

#include "kc/core/Zip.hpp"

struct StringTests : ::testing::Test {
};

TEST_F(StringTests, givenStringToStripWithoutCharactersToStrip_whenStriping_shouldNotChangeString) {
    std::string str = "abcdefg";
    std::string copy = str;

    kc::core::strip(str, '_');

    EXPECT_EQ(str, copy);
}

TEST_F(StringTests, givenEmptyPath_whenExtractingName_shouldReturnEmptyString) {
    const std::string emptyString = "";

    EXPECT_EQ(kc::core::extractNameFromPath(emptyString), emptyString);
}

TEST_F(StringTests, givenPathWithoutSlash_whenExtractingName_shouldReturnOriginalString) {
    const std::string invalidPath = "hello_my_name_is";

    EXPECT_EQ(kc::core::extractNameFromPath(invalidPath), invalidPath);
}

TEST_F(StringTests, givenPath_whenExtractingName_shouldReturnName) {
    const std::string fileName = "fileName.txt";
    std::string path = "/usr/local/" + fileName;

    EXPECT_EQ(kc::core::extractNameFromPath(path), fileName);
}

TEST_F(StringTests, givenStringToStrip_whenStriping_shouldStrip) {
    std::string str = "abcde___";
    std::string expected = "abcde";

    kc::core::strip(str, '_');

    EXPECT_EQ(str, expected);
}

TEST_F(StringTests, givenStringToStripWithCharacterInside_whenStriping_shouldNotStripInsideCharacter) {
    std::string str = "a_bcde___";
    std::string expected = "a_bcde";

    kc::core::strip(str, '_');

    EXPECT_EQ(str, expected);
}

TEST_F(StringTests, givenEmptyString_whenSplitting_shouldReturnEmptyVector) {
    std::string stringToSplit = "";

    EXPECT_EQ(kc::core::split(stringToSplit, '_').size(), 0);
}

TEST_F(StringTests, givenStringWithoutDelimiter_whenSplitting_shouldReturnSingleElement) {
    std::string stringToSplit = "ab cd ef";

    auto splitted = kc::core::split(stringToSplit, '_');

    ASSERT_EQ(splitted.size(), 1);
    EXPECT_EQ(splitted[0], stringToSplit);
}

TEST_F(StringTests, givenStringWithDelimiter_whenSplitting_shouldSplitCorrectly) {
    std::vector<std::string> parts = {
        "ab", "cd", "ef", "gh"
    };

    const char delimiter = '-';

    std::string stringToSplit = std::accumulate(parts.begin(), parts.end(), std::string {}, [&](auto pre, auto word) {
        return pre + word + delimiter;
    });

    stringToSplit.pop_back();

    auto splitted = kc::core::split(stringToSplit, delimiter);

    ASSERT_EQ(splitted.size(), parts.size());

    for (auto&& [received, expected] : kc::core::zip(splitted, parts))
        EXPECT_EQ(received, expected);
}

TEST_F(StringTests, givenStringWithDelimiterAtTheBegining_shouldReturnSingleElement) {
    std::string expected = "ab cd ef";
    const char delimiter = '_';

    std::string stringToSplit = delimiter + expected;

    auto splitted = kc::core::split(stringToSplit, delimiter);

    ASSERT_EQ(splitted.size(), 1);
    EXPECT_EQ(splitted[0], expected);
}

TEST_F(StringTests, givenStringWithDelimiterAtTheEnd_shouldReturnSingleElement) {
    std::string expected = "ab cd ef";
    const char delimiter = '_';

    std::string stringToSplit = expected + delimiter;

    auto splitted = kc::core::split(stringToSplit, delimiter);

    ASSERT_EQ(splitted.size(), 1);
    EXPECT_EQ(splitted[0], expected);
}