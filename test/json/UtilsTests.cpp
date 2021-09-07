#include "kc/json/Utils.hpp"

#include <gtest/gtest.h>

#include "kc/json/Json.h"

using namespace kc;
using namespace testing;

struct NodeHelperTests : Test, json::NodeHelper<json::JsonError> {
    void SetUp() override {
        root["int"] = 3;
        root["bool"] = false;
        root["float"] = 1.5f;
    }

    json::Node root;
};

TEST_F(NodeHelperTests, givenNodeWithoutField_whenGettingField_shouldThrow) {
    EXPECT_THROW(
        fieldFrom(root).ofName("NotExistingNode"), json::JsonError);
}

TEST_F(NodeHelperTests, givenNodeWithField_whenSettingUpNameTwice_shouldThrow) {
    EXPECT_THROW(
        fieldFrom(root).ofName("int").ofName("int"), json::JsonError);
}

TEST_F(NodeHelperTests, givenNode_whenSettingTypeWithoutSettingName_shouldThrow) {
    EXPECT_THROW(
        fieldFrom(root).ofType<int>(), json::JsonError);
}

TEST_F(NodeHelperTests, givenNode_whenGettingValueWithWrongType_shouldThrow) {
    EXPECT_THROW(
        fieldFrom(root).ofName("float").ofType<int>(), json::JsonError);
}

TEST_F(NodeHelperTests, givenNode_whenGettingValueCorrectly_shouldReturnCorrectValue) {
    static constexpr int value = 5;
    root["val"] = value;

    EXPECT_EQ(
        fieldFrom(root).ofName("val").ofType<int>().get(), value);
}

TEST_F(NodeHelperTests, givenNode_whenGettingValueOutOfGivenRange_shouldThrow) {
    EXPECT_THROW(
        fieldFrom(root).ofName("int").ofType<int>().min(5).get(), json::JsonError);
    EXPECT_THROW(
        fieldFrom(root).ofName("int").ofType<int>().max(0).get(), json::JsonError);
    EXPECT_THROW(
        fieldFrom(root).ofName("int").ofType<int>().inRange(0, 1).get(), json::JsonError);
}