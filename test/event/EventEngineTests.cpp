#include <gtest/gtest.h>

#include <memory>

#include "kc/event/EventEngine.h"

namespace {

class EventEngineTests : public testing::Test {
protected:
    kc::event::EventEngine eventEngine;
};

class ConcreteEventListener : public kc::event::EventListener {
public:
    ConcreteEventListener()
        : kc::event::EventListener(ident) {
    }

    void handleEvents(const kc::event::EventProvider&) {
    }

    static inline std::string ident = "ident";
};

TEST_F(EventEngineTests, givenEventEngine_whenCheckingIfUnregisteredListenerIsRegistered_shouldReturnFalse) {
    EXPECT_FALSE(eventEngine.isListenerRegistered("notExistingListener"));
}

TEST_F(EventEngineTests, givenEventEngine_whenUnregisteringNotExistingListener_shouldThrow) {
    ASSERT_THROW(eventEngine.unregisterEventListener("notExistingListener"), ListenerNotFound);
}

TEST_F(EventEngineTests, givenEventEngine_whenRegisteringListener_shouldRegister) {
    auto listener = std::make_shared<ConcreteEventListener>();
    eventEngine.registerEventListener(listener.get());

    EXPECT_TRUE(eventEngine.isListenerRegistered(ConcreteEventListener::ident));
}

TEST_F(EventEngineTests, giventEventEngine_whenUnregisteringService_shouldUnregister) {
    auto listener = std::make_shared<ConcreteEventListener>();
    eventEngine.registerEventListener(listener.get());

    EXPECT_TRUE(eventEngine.isListenerRegistered(ConcreteEventListener::ident));

    eventEngine.unregisterEventListener(listener.get());

    EXPECT_FALSE(eventEngine.isListenerRegistered(ConcreteEventListener::ident));
}

// TODO: rewrite this test
// TEST_F(EventEngineTests, givenEventEngine_whenRegisteringListenerTwice_shouldThrow) {
//     auto listener = std::make_shared<ConcreteEventListener>();
//     eventEngine.registerEventListener(listener.get());

//     ASSERT_THROW(eventEngine.registerEventListener(listener.get()), ListenerAlreadyRegistered);
// }

struct EventA : kc::event::EventBase<EventA> { };
struct EventB : kc::event::EventBase<EventB> { };

struct Listener1 : public kc::event::EventListener {
    using kc::event::EventListener::EventListener;

    void handleEvents(const kc::event::EventProvider& eventProvider) {
        for (auto& event : eventProvider.getAll())
            ++events;
    }

    int events = 0;
};

struct Listener2 : public kc::event::EventListener {
    using kc::event::EventListener::EventListener;

    void handleEvents(const kc::event::EventProvider& eventProvider) {
        for (auto& event : eventProvider.getAll())
            ++events;
    }

    int events = 0;
};

class EventEngineEventsTest : public testing::Test {
protected:
    void SetUp() override {
        m_listener1 = std::make_shared<Listener1>("listener1");
        m_listener2 = std::make_shared<Listener2>("listener2");

        m_listener1->events = 0;
        m_listener2->events = 0;

        m_eventEngine.registerEventListener(m_listener1.get());
        m_eventEngine.registerEventListener(m_listener2.get());
    }

    kc::event::EventEngine m_eventEngine;

    std::shared_ptr<Listener1> m_listener1;
    std::shared_ptr<Listener2> m_listener2;
};

TEST_F(EventEngineEventsTest, givenEngine_whenSpreadingEventsWithoutAnyEventEmitted_everyListenerShouldNotGetEvent) {
    m_eventEngine.spreadEvents();

    EXPECT_EQ(m_listener1->events, 0);
    EXPECT_EQ(m_listener2->events, 0);
}

TEST_F(EventEngineEventsTest, givenEngine_whenSpreadingEvents_everyListenerShouldGetOneEvent) {
    auto eventEmitter = m_eventEngine.createEmitter();
    eventEmitter->emit<EventA>().toAll();

    m_eventEngine.spreadEvents();

    EXPECT_EQ(m_listener1->events, 1);
    EXPECT_EQ(m_listener2->events, 1);
}

TEST_F(EventEngineEventsTest, givenEngine_whenSpreadingEventsTwiceWithoutNewEvents_everyListenerShouldGetOneEvent) {
    auto eventEmitter = m_eventEngine.createEmitter();
    eventEmitter->emit<EventA>().toAll();

    m_eventEngine.spreadEvents();
    m_eventEngine.spreadEvents();

    EXPECT_EQ(m_listener1->events, 1);
    EXPECT_EQ(m_listener2->events, 1);
}

TEST_F(EventEngineEventsTest, givenEngine_whenSpreadingEventsEmittedToOnlyOneListener_onlyThisListenerShouldGetEvent) {
    auto eventEmitter = m_eventEngine.createEmitter();
    eventEmitter->emit<EventA>().to("listener1");

    m_eventEngine.spreadEvents();

    EXPECT_EQ(m_listener1->events, 1);
    EXPECT_EQ(m_listener2->events, 0);
}

struct Listener3 : public kc::event::EventListener {
    using kc::event::EventListener::EventListener;

    void handleEvents(const kc::event::EventProvider& eventProvider) {
        for (auto& event : eventProvider.getAll())
            ++events;
    }

    int events = 0;
};

TEST_F(EventEngineEventsTest, giventEventListenerRegisteredLate_shouldGetEvent) {
    auto eventEmitter = m_eventEngine.createEmitter();

    auto listener3 = std::make_shared<Listener3>("listener3");
    m_eventEngine.registerEventListener(listener3.get());

    eventEmitter->emit<EventA>().toAll();

    m_eventEngine.spreadEvents();
    EXPECT_EQ(listener3->events, 1);

    eventEmitter->emit<EventA>().toAll();

    m_eventEngine.spreadEvents();
    EXPECT_EQ(listener3->events, 2);
}
}
