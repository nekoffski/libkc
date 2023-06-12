#include <gtest/gtest.h>

#include "kc/event2/EventManager.hpp"

using namespace testing;
using namespace kc;

struct TestEventA {
    int a;
};

struct TestEventB {
    float a;
};

struct EventManagerTests : Test {
    event2::EventManager eventManager;
};

TEST_F(EventManagerTests, givenEvents_whenEmitted_dispatchShouldHandleEachEventCorrectly) {
    int aCalls = 0;
    int bCalls = 0;

    {
        eventManager.on<TestEventA>([&](TestEventA* event) { ++aCalls; });
        eventManager.on<TestEventB>([&](TestEventB* event) { ++bCalls; });
    }

    eventManager.emit<TestEventA>(1);
    eventManager.emit<TestEventB>(1.5f);
    eventManager.emit<TestEventB>(2.5f);

    eventManager.dispatch();

    ASSERT_EQ(aCalls, 1);
    ASSERT_EQ(bCalls, 2);
}
