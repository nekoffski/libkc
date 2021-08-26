#include "kc/containers/RingBuffer.hpp"

#include <gtest/gtest.h>

using namespace kc::containers;

struct Type {
    int x;

    bool operator<=>(const Type& oth) const = default;
};

struct RingBufferTests : testing::Test {
    RingBuffer<Type> ringBuffer;
};

TEST_F(RingBufferTests, givenRingBuffer_whenPushingMoreItemsThanCapacity_shouldAllowInsertingAnotherValues) {
    constexpr int capacity = 10;
    RingBuffer<int> buffer(capacity);

    constexpr int cycles = capacity * capacity * capacity;

    for (int i = 0; i < cycles; ++i) {
        for (int j = 0; j < 5; ++j)
            buffer.push(i * cycles + j);

        for (int j = 0; j < 5; ++j)
            EXPECT_EQ(buffer.pop(), i * cycles + j);
    }
}

TEST_F(RingBufferTests, givenEmptyRingBuffer_whenCheckingIfEmpty_shouldReturnTrue) {
    EXPECT_TRUE(ringBuffer.isEmpty());
}

TEST_F(RingBufferTests, givenDefaultRingBuffer_whenCheckignCapacity_shouldReturnDefaultValue) {
    EXPECT_EQ(RingBuffer<int> {}.getCapacity(), RingBuffer<int>::defaultCapacity);
}

TEST_F(RingBufferTests, givenRingBufferWithCustomCapacity_whenCheckingCapacity_shouldReturnCorrectValue) {
    const auto capacity = 1ll;
    EXPECT_EQ(RingBuffer<int> { capacity }.getCapacity(), capacity);
}

TEST_F(RingBufferTests, givenNonEmptyRingBuffer_whenCheckingIfEmpty_shouldReturnFalse) {
    ASSERT_TRUE(ringBuffer.isEmpty());

    ringBuffer.push({ 1 });
    EXPECT_FALSE(ringBuffer.isEmpty());

    [[maybe_unused]] auto val = ringBuffer.pop();
    EXPECT_TRUE(ringBuffer.isEmpty());
}

TEST_F(RingBufferTests, givenRingBuffer_whenPushedValue_shouldAllowToPopValue) {
    Type value { 5 };
    ringBuffer.push(value);

    EXPECT_EQ(ringBuffer.pop(), value);
}

TEST_F(RingBufferTests, givenRingBuffer_whenPushedValueByMove_shouldAllowToPopValue) {
    constexpr static int x = 5;
    Type value { x };
    ringBuffer.push(std::move(value));

    EXPECT_EQ(ringBuffer.pop(), value);
}

TEST_F(RingBufferTests, givenRingBuffer_whenPushedValueByTemporaryObject_shouldAllowToPopValue) {
    constexpr static int x = 5;
    ringBuffer.push(Type { x });

    EXPECT_EQ(ringBuffer.pop().x, x);
}