#include <gtest/gtest.h>

#include "kc/containers/ThreadSafeQueue.hpp"

using namespace kc::containers;

struct ThreadSafeQueueTests : testing::Test {
    ThreadSafeQueue<int> q;
};

TEST_F(ThreadSafeQueueTests, givenEmptyQueue_whenGettingSize_shouldReturnZero) {
    EXPECT_EQ(q.getSize(), 0);
}

TEST_F(ThreadSafeQueueTests, givenEmptyQueue_whenTryPopping_shouldReturnNull) {
    EXPECT_FALSE(q.tryPop().has_value());
}

TEST_F(ThreadSafeQueueTests, givenQueue_whenPushingElement_shouldIncreaseSize) {
    q.push(1);
    EXPECT_EQ(q.getSize(), 1);
}

TEST_F(ThreadSafeQueueTests, givenQueue_whenTryPushingElement_shouldIncreaseSize) {
    q.tryPush(1);
    EXPECT_EQ(q.getSize(), 1);
}

TEST_F(ThreadSafeQueueTests, givenQueue_whenPushingElementAndPopping_shouldBeEqual) {
    q.push(1337);
    ASSERT_EQ(q.getSize(), 1);
    auto r = q.pop();
    ASSERT_TRUE(r.has_value());
    EXPECT_EQ(r.value(), 1337);
}

TEST_F(ThreadSafeQueueTests, givenQueue_whenTryPushingElementAndPopping_shouldBeEqual) {
    q.tryPush(1337);
    ASSERT_EQ(q.getSize(), 1);
    auto r = q.pop();
    ASSERT_TRUE(r.has_value());
    EXPECT_EQ(r.value(), 1337);
}
