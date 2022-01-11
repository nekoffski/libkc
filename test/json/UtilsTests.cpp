#include "kc/json/Utils.hpp"

#include <glm/glm.hpp>
#include <gtest/gtest.h>

#include "kc/json/Json.h"

using namespace kc;
using namespace testing;

struct NodeHelperTests : Test, json::NodeHelper<json::JsonError> {
    void SetUp() override {
        root["int"] = 3;
        root["bool"] = false;
        root["float"] = 1.5f;

        root["array"] = Json::arrayValue;
        root["array"].append(1);
        root["array"].append(2);
        root["array"].append(3);
    }

    json::Node root;
};

TEST_F(NodeHelperTests, givenNodeWithoutField_whenGettingField_shouldThrow) {
    EXPECT_THROW(
        fieldFrom(root).withName("NotExistingNode"), json::JsonError);
}

TEST_F(NodeHelperTests, givenNodeWithField_whenSettingUpNameTwice_shouldThrow) {
    EXPECT_THROW(
        fieldFrom(root).withName("int").withName("int"), json::JsonError);
}

TEST_F(NodeHelperTests, givenNode_whenSettingTypeWithoutSettingName_shouldThrow) {
    EXPECT_THROW(
        fieldFrom(root).ofType<int>(), json::JsonError);
}

TEST_F(NodeHelperTests, givenNode_whenGettingValueWithWrongType_shouldThrow) {
    EXPECT_THROW(
        fieldFrom(root).withName("float").ofType<int>(), json::JsonError);
}

TEST_F(NodeHelperTests, givenNode_whenGettingValueCorrectly_shouldReturnCorrectValue) {
    EXPECT_EQ(
        fieldFrom(root).withName("int").ofType<int>().get(), 3);

    EXPECT_EQ(
        fieldFrom(root).withName("bool").ofType<bool>().get(), false);

    EXPECT_EQ(
        fieldFrom(root).withName("float").ofType<float>().get(), 1.5f);
}

TEST_F(NodeHelperTests, givenNodeWithValueOutOfRange_whenGettingValue_shouldThrow) {
    EXPECT_THROW(
        fieldFrom(root).withName("int").ofType<int>().min(5).get(), json::JsonError);
    EXPECT_THROW(
        fieldFrom(root).withName("int").ofType<int>().max(0).get(), json::JsonError);
    EXPECT_THROW(
        fieldFrom(root).withName("int").ofType<int>().inRange(0, 1).get(), json::JsonError);
}

TEST_F(NodeHelperTests, givenStringNodeWithSizeOutOfRange_whenGettingValue_shouldThrow) {
    static const std::string str = "some string";
    static const auto n = str.size();

    root["str"] = str;

    EXPECT_THROW(
        fieldFrom(root).withName("str").ofType<std::string>().nonEmpty().minSize(n + 1).get(), json::JsonError);

    EXPECT_THROW(
        fieldFrom(root).withName("str").ofType<std::string>().nonEmpty().maxSize(n - 1).get(), json::JsonError);

    EXPECT_THROW(
        fieldFrom(root).withName("str").ofType<std::string>().nonEmpty().sizeInRange(0, 1).get(), json::JsonError);
}

TEST_F(NodeHelperTests, givenArrayNodeWithSizeOutOfRange_whenGettingValue_shouldThrow) {
    EXPECT_THROW(
        fieldFrom(root).withName("array").asArray().nonEmpty().sizeInRange(0, 2).get(), json::JsonError);

    EXPECT_THROW(
        fieldFrom(root).withName("array").asArray().nonEmpty().minSize(55).get(), json::JsonError);

    EXPECT_THROW(
        fieldFrom(root).withName("array").asArray().nonEmpty().maxSize(1).get(), json::JsonError);
}

TEST_F(NodeHelperTests, givenGlm2Vector_whenDeserializing_shouldDeserializeCorrectly) {
    root["array"].resize(2);
    EXPECT_NO_THROW({
        auto vector = fieldFrom(root).withName("array").ofType<glm::vec2>().get();

        EXPECT_EQ(vector.x, 1);
        EXPECT_EQ(vector.y, 2);
    });
}

TEST_F(NodeHelperTests, givenGlmVectorWithNotEnoughFields_whenDeserializing_shouldThrow) {
    root["array"].resize(1);
    EXPECT_THROW({
        fieldFrom(root).withName("array").ofType<glm::vec2>().get();
    },
        json::JsonError);
    EXPECT_THROW({
        fieldFrom(root).withName("array").ofType<glm::vec3>().get();
    },
        json::JsonError);
    EXPECT_THROW({
        fieldFrom(root).withName("array").ofType<glm::vec4>().get();
    },
        json::JsonError);
}

TEST_F(NodeHelperTests, givenGlmVectorWithTooManyFields_whenDeserializing_shouldThrow) {
    root["array"].resize(5);
    EXPECT_THROW({
        fieldFrom(root).withName("array").ofType<glm::vec2>().get();
    },
        json::JsonError);
    EXPECT_THROW({
        fieldFrom(root).withName("array").ofType<glm::vec3>().get();
    },
        json::JsonError);
    EXPECT_THROW({
        fieldFrom(root).withName("array").ofType<glm::vec4>().get();
    },
        json::JsonError);
}

TEST_F(NodeHelperTests, givenGlmVectorWithInvalidType_whenDeserializing_shouldThrow) {
    root["array"].resize(2);
    root["array"].append("abcd");
    EXPECT_THROW({
        fieldFrom(root).withName("array").ofType<glm::vec2>().get();
    },
        json::JsonError);
    EXPECT_THROW({
        fieldFrom(root).withName("array").ofType<glm::vec3>().get();
    },
        json::JsonError);
    EXPECT_THROW({
        fieldFrom(root).withName("array").ofType<glm::vec4>().get();
    },
        json::JsonError);
}

TEST_F(NodeHelperTests, givenGlm3Vector_whenDeserializing_shouldDeserializeCorrectly) {
    EXPECT_NO_THROW({
        auto vector = fieldFrom(root).withName("array").ofType<glm::vec3>().get();

        EXPECT_EQ(vector.x, 1);
        EXPECT_EQ(vector.y, 2);
        EXPECT_EQ(vector.z, 3);
    });
}

TEST_F(NodeHelperTests, givenGlm4Vector_whenDeserializing_shouldDeserializeCorrectly) {
    root["array"].append(4);

    EXPECT_NO_THROW({
        auto vector = fieldFrom(root).withName("array").ofType<glm::vec4>().get();

        EXPECT_EQ(vector.x, 1);
        EXPECT_EQ(vector.y, 2);
        EXPECT_EQ(vector.z, 3);
        EXPECT_EQ(vector.w, 4);
    });
}

TEST_F(NodeHelperTests, givenArrayOfDifferentTypeThanExpected_whenGettingValue_shouldThrow) {
    EXPECT_THROW(
        fieldFrom(root).withName("array").asArray().ofType<std::string>().get(), json::JsonError);
}

TEST_F(NodeHelperTests, givenObjectNode_whenGettingValue_shouldReturnObject) {
    root["obj"] = Json::objectValue;

    EXPECT_TRUE(
        fieldFrom(root).withName("obj").asObject().get().isObject());
}

TEST_F(NodeHelperTests, givenArray_whenIterating_shouldNotThrow) {
    int x = 1;

    EXPECT_NO_THROW({
        for (auto& value : fieldFrom(root).withName("array").asArray().get()) {
            EXPECT_EQ(value, x++);
        }
    });
}

TEST_F(NodeHelperTests, givenEmptyField_whenExpectingNonEmpty_shouldThrow) {
    root["str"] = "";
    root["arr"] = Json::arrayValue;

    EXPECT_THROW(
        fieldFrom(root).withName("str").ofType<std::string>().nonEmpty().get(), json::JsonError);

    EXPECT_THROW(
        fieldFrom(root).withName("arr").asArray().nonEmpty().get(), json::JsonError);
}