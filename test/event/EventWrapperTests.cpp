#include <gtest/gtest.h>

#include "Event.h"
#include "kc/event/EventWrapper.hpp"

using namespace kc;
using namespace testing;

struct EventWrapperTests : Test {
    EventWrapperTests() : event(value), eventWrapper(event) {}

    static constexpr int value = 1337;

    Event event;
    event::EventWrapper eventWrapper;
};

TEST_F(EventWrapperTests, givenEventWrapper_whenCheckingIfIsHandled_shouldReturnFalse) {
    EXPECT_FALSE(eventWrapper.isHandled());
}

TEST_F(EventWrapperTests, givenHandledEventWrapper_whenCheckingIfIsHandled_shouldReturnTrue) {
    eventWrapper.handled();

    EXPECT_TRUE(eventWrapper.isHandled());
}

TEST_F(EventWrapperTests,
       givenEventWrapper_whenCheckingIfTypeIsCorrectWithCorrectType_shouldReturnTrue) {
    EXPECT_TRUE(eventWrapper.is<Event>());
}

TEST_F(EventWrapperTests,
       givenEventWrapper_whenCheckingIfTypeIsCorrectWithInorrectType_shouldReturnFalse) {
    EXPECT_FALSE(eventWrapper.is<int>());
}

TEST_F(EventWrapperTests, givenEventWrapper_whenGettingValue_shouldReturnCorrectValue) {
    auto event = eventWrapper.get<Event>();
    EXPECT_EQ(event.value, value);
}
