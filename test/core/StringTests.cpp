#include <gtest/gtest.h>

#include <numeric>

#include "kc/core/String.h"
#include "kc/core/Zip.hpp"

using namespace kc::core;

struct StringTests : ::testing::Test {};

TEST_F(StringTests, givenStringToStripWithoutCharactersToStrip_whenStriping_shouldNotChangeString) {
    std::string str  = "abcdefg";
    std::string copy = str;

    strip(str, '_');

    EXPECT_EQ(str, copy);
}

TEST_F(StringTests, givenEmptyPath_whenExtractingName_shouldReturnEmptyString) {
    const std::string emptyString = "";

    EXPECT_EQ(extractNameFromPath(emptyString, ExtractingMode::withoutExtension), emptyString);
    EXPECT_EQ(extractNameFromPath(emptyString, ExtractingMode::withExtension), emptyString);
}

TEST_F(StringTests, givenPathWithoutSlash_whenExtractingName_shouldReturnOriginalString) {
    const std::string invalidPath = "hello_my_name_is";

    EXPECT_EQ(extractNameFromPath(invalidPath, ExtractingMode::withoutExtension), invalidPath);
    EXPECT_EQ(extractNameFromPath(invalidPath, ExtractingMode::withExtension), invalidPath);
}

TEST_F(StringTests, givenPath_whenExtractingName_shouldReturnName) {
    const std::string fileName  = "fileName";
    const std::string extension = ".txt";
    std::string path            = "/usr/local/" + fileName + extension;

    EXPECT_EQ(extractNameFromPath(path, ExtractingMode::withExtension), fileName + extension);
    EXPECT_EQ(extractNameFromPath(path, ExtractingMode::withoutExtension), fileName);
}

TEST_F(StringTests, givenStringToStrip_whenStriping_shouldStrip) {
    std::string str      = "abcde___";
    std::string expected = "abcde";

    strip(str, '_');

    EXPECT_EQ(str, expected);
}

TEST_F(
    StringTests, givenStringToStripWithCharacterInside_whenStriping_shouldNotStripInsideCharacter
) {
    std::string str      = "a_bcde___";
    std::string expected = "a_bcde";

    strip(str, '_');

    EXPECT_EQ(str, expected);
}

TEST_F(StringTests, givenEmptyString_whenSplitting_shouldReturnEmptyVector) {
    std::string stringToSplit = "";

    EXPECT_EQ(split(stringToSplit, '_').size(), 0);
}

TEST_F(StringTests, givenStringWithoutDelimiter_whenSplitting_shouldReturnSingleElement) {
    std::string stringToSplit = "ab cd ef";

    auto splitted = split(stringToSplit, '_');

    ASSERT_EQ(splitted.size(), 1);
    EXPECT_EQ(splitted[0], stringToSplit);
}

TEST_F(StringTests, givenStringWithDelimiter_whenSplitting_shouldSplitCorrectly) {
    std::vector<std::string> parts = {"enum", "Stats", "{{{{{", "gheee"};

    const char delimiter = '-';

    std::string stringToSplit =
        std::accumulate(parts.begin(), parts.end(), std::string{}, [&](auto pre, auto word) {
            return pre + word + delimiter;
        });

    stringToSplit.pop_back();

    auto splitted = split(stringToSplit, delimiter);

    ASSERT_EQ(splitted.size(), parts.size());

    for (auto&& [received, expected] : zip(splitted, parts)) EXPECT_EQ(received, expected);
}

TEST_F(StringTests, givenStringWithDelimiterAtTheBegining_shouldReturnSingleElement) {
    std::string expected = "ab cd ef";
    const char delimiter = '_';

    std::string stringToSplit = delimiter + expected;

    auto splitted = split(stringToSplit, delimiter);

    ASSERT_EQ(splitted.size(), 1);
    EXPECT_EQ(splitted[0], expected);
}

TEST_F(StringTests, givenStringWithDelimiterAtTheEnd_shouldReturnSingleElement) {
    std::string expected = "ab cd ef";
    const char delimiter = '_';

    std::string stringToSplit = expected + delimiter;

    auto splitted = split(stringToSplit, delimiter);

    ASSERT_EQ(splitted.size(), 1);
    EXPECT_EQ(splitted[0], expected);
}

TEST_F(StringTests, givenStringWithMultipleDelimiter_shouldReturnCorrectElements) {
    const char delimiter = '_';
    std::string a        = "aa";
    std::string b        = "bb";

    std::string stringToSplit = a + delimiter + delimiter + b;

    auto splitted = split(stringToSplit, delimiter);

    ASSERT_EQ(splitted.size(), 2);
    EXPECT_EQ(splitted[0], a);
    EXPECT_EQ(splitted[1], b);
}