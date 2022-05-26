#include <gtest/gtest.h>

#include "Event.h"
#include "kc/event/EventManager.hpp"
#include "mocks/EventObserverMock.hpp"

using namespace testing;
using namespace kc;

struct EventManagerTests : Test {
    event::EventManager eventManager;
    EventObserverMock observer;
    EventObserverMock observer2;
};

TEST_F(
    EventManagerTests,
    givenEventManagerWithoutObserversRegistered_whenCheckingObserversCount_shouldReturnZero
) {
    EXPECT_EQ(eventManager.getObserversCount(), 0);
}

TEST_F(EventManagerTests, givenEventManager_whenUnregisteringNotRegisteredObserver_shouldNotThrow) {
    EXPECT_NO_THROW({ eventManager.unregisterObserver(nullptr); });
}

TEST_F(
    EventManagerTests,
    givenEventManagerWithRegisteredObserver_whenUnregisteringObserver_shouldDecrementObserversCount
) {
    ASSERT_EQ(eventManager.getObserversCount(), 0);

    eventManager.registerObserver(&observer);
    ASSERT_EQ(eventManager.getObserversCount(), 1);

    eventManager.unregisterObserver(&observer);
    ASSERT_EQ(eventManager.getObserversCount(), 0);
}

TEST_F(EventManagerTests, givenEventManager_whenRegisteringObserver_shouldIncrementObserversCount) {
    ASSERT_EQ(eventManager.getObserversCount(), 0);

    eventManager.registerObserver(&observer);
    EXPECT_EQ(eventManager.getObserversCount(), 1);
}

TEST_F(EventManagerTests, givenEventManager_whenRegisteringObserverTwice_shouldThrow) {
    eventManager.registerObserver(&observer);
    EXPECT_ANY_THROW({ eventManager.registerObserver(&observer); });
}

TEST_F(EventManagerTests, givenObserver_whenEmittingEvent_shouldCallOnEvent) {
    eventManager.registerObserver(&observer);

    EXPECT_CALL(observer, onEvent(_)).Times(1);
    eventManager.emitEvent<Event>(1);
}

TEST_F(EventManagerTests, givenTwoObservers_whenEmittingEvent_shouldCallOnEvent) {
    eventManager.registerObserver(&observer);
    eventManager.registerObserver(&observer2);

    EXPECT_CALL(observer, onEvent(_)).Times(1);
    EXPECT_CALL(observer2, onEvent(_)).Times(1);
    eventManager.emitEvent<Event>(1);
}

TEST_F(
    EventManagerTests, givenTwoObservers_whenEmittingEventAfterUnregistering_shouldCallNotOnEvent
) {
    eventManager.registerObserver(&observer);
    eventManager.registerObserver(&observer2);

    EXPECT_CALL(observer, onEvent(_)).Times(2);
    EXPECT_CALL(observer2, onEvent(_)).Times(1);
    eventManager.emitEvent<Event>(1);
    eventManager.unregisterObserver(&observer2);

    Event event{1};
    eventManager.emitEvent(event);
}

TEST_F(
    EventManagerTests, givenTwoObserver_whenFirstHandleEvent_shouldNotCallOnEventForSecondObserver
) {
    eventManager.registerObserver(&observer);
    eventManager.registerObserver(&observer2);

    ON_CALL(observer, onEvent).WillByDefault([](event::EventWrapper& eventWrapper) {
        eventWrapper.handled();
    });

    EXPECT_CALL(observer, onEvent(_)).Times(1);
    EXPECT_CALL(observer2, onEvent(_)).Times(0);

    eventManager.emitEvent<Event>(1);
}

TEST_F(EventManagerTests, givenObserver_whenEmittingEvent_shouldEmitCorrectEventToObserver) {
    eventManager.registerObserver(&observer);

    Event event{1337};

    ON_CALL(observer, onEvent).WillByDefault([&](event::EventWrapper& eventWrapper) {
        ASSERT_TRUE(eventWrapper.is<Event>());
        EXPECT_EQ(event.value, eventWrapper.get<Event>().value);
    });

    EXPECT_CALL(observer, onEvent(_)).Times(1);
    eventManager.emitEvent(event);
}
