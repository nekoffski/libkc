#include <gtest/gtest.h>

#include <typeindex>

#include "kc/event/EventEmitter.h"

namespace {

const std::string ident1 = "ident1";
const std::string ident2 = "ident2";

const std::type_index defCategoryType = typeid(kc::event::DefaultCategory);

struct EventA : public kc::event::EventBase<EventA> { };
struct EventB : public kc::event::EventBase<EventB> { };

class EventEmitterTests : public testing::Test {
protected:
    EventEmitterTests() {
        m_eventEmitter = std::make_shared<kc::event::EventEmitter>(m_eventContainer);

        m_eventContainer[ident1] = kc::event::CategoryToEventQueue {};
        m_eventContainer[ident2] = kc::event::CategoryToEventQueue {};
    }

    kc::event::EventContainer m_eventContainer;
    std::shared_ptr<kc::event::EventEmitter> m_eventEmitter;
};

TEST_F(EventEmitterTests, whenEmittingEventToNotExistingListener_shouldThrowError) {
    ASSERT_THROW(m_eventEmitter->emit<EventA>().to("NotExistingListener"), kc::event::ListenerNotFound);
}

TEST_F(EventEmitterTests, givenEventResult_whenWaitingWithTimeout_shouldThrowError) {
    auto result = m_eventEmitter->emit<EventA>().to(ident1);
    EXPECT_FALSE(result.isReady());
    ASSERT_THROW(result.wait(5ms), kc::event::ResultTimeout);
}

TEST_F(EventEmitterTests, givenEventResult_whenWaitingAndEventIsSet_shouldReturnEvent) {
    auto result = m_eventEmitter->emit<EventA>().to(ident1);
    EXPECT_FALSE(result.isReady());

    auto& m1 = m_eventContainer[ident1];
    auto& event = m1[defCategoryType][0];
    event->setResult<EventB>();

    EXPECT_TRUE(result.isReady());

    auto eventResult = result.wait(1ms);
    EXPECT_TRUE(eventResult->is<EventB>());
}

TEST_F(EventEmitterTests, whenEmitting_everyListenerShouldReceiveEvent) {
    m_eventEmitter->emit<EventA>().toAll();

    auto& m1 = m_eventContainer[ident1];
    auto& m2 = m_eventContainer[ident2];

    ASSERT_EQ(m1.size(), 1);
    ASSERT_EQ(m2.size(), 1);

    auto& defQueue1 = m1[defCategoryType];
    auto& defQueue2 = m2[defCategoryType];

    ASSERT_EQ(defQueue1.size(), 1);
    ASSERT_EQ(defQueue2.size(), 1);

    ASSERT_EQ(defQueue1[0]->getEventTypeIndex(), std::type_index { typeid(EventA) });
    ASSERT_EQ(defQueue2[0]->getEventTypeIndex(), std::type_index { typeid(EventA) });
}

TEST_F(EventEmitterTests, whenEmitting2Events_everyListenerShouldReceiveEvent) {
    m_eventEmitter->emit<EventA>().toAll();
    m_eventEmitter->emit<EventB>().toAll();

    auto& m1 = m_eventContainer[ident1];
    auto& m2 = m_eventContainer[ident2];

    ASSERT_EQ(m1.size(), 1);
    ASSERT_EQ(m2.size(), 1);

    auto& defQueue1 = m1[defCategoryType];
    auto& defQueue2 = m2[defCategoryType];

    ASSERT_EQ(defQueue1.size(), 2);
    ASSERT_EQ(defQueue2.size(), 2);

    ASSERT_EQ(defQueue1[0]->getEventTypeIndex(), std::type_index { typeid(EventA) });
    ASSERT_EQ(defQueue2[0]->getEventTypeIndex(), std::type_index { typeid(EventA) });

    ASSERT_EQ(defQueue1[1]->getEventTypeIndex(), std::type_index { typeid(EventB) });
    ASSERT_EQ(defQueue2[1]->getEventTypeIndex(), std::type_index { typeid(EventB) });
}

TEST_F(EventEmitterTests, whenEmittingTo2Listeners_only2ListenersShouldReceiveEvent) {
    const std::string ident3 = "ident3";

    m_eventContainer[ident3] = kc::event::CategoryToEventQueue {};

    m_eventEmitter->emit<EventA>().to({ ident1, ident2 });

    auto& m1 = m_eventContainer[ident1];
    auto& m2 = m_eventContainer[ident2];
    auto& m3 = m_eventContainer[ident3];

    ASSERT_EQ(m1.size(), 1);
    ASSERT_EQ(m2.size(), 1);
    ASSERT_EQ(m3.size(), 0);

    auto& defQueue1 = m1[defCategoryType];

    ASSERT_EQ(defQueue1.size(), 1);
    ASSERT_EQ(defQueue1[0]->getEventTypeIndex(), std::type_index { typeid(EventA) });

    auto& defQueue2 = m2[defCategoryType];

    ASSERT_EQ(defQueue2.size(), 1);
    ASSERT_EQ(defQueue2[0]->getEventTypeIndex(), std::type_index { typeid(EventA) });
}

TEST_F(EventEmitterTests, whenEmittingToListener_onlyListenerShouldReceiveEvent) {
    m_eventEmitter->emit<EventA>().to(ident1);

    auto& m1 = m_eventContainer[ident1];
    auto& m2 = m_eventContainer[ident2];

    ASSERT_EQ(m1.size(), 1);
    ASSERT_EQ(m2.size(), 0);

    auto& defQueue1 = m1[defCategoryType];

    ASSERT_EQ(defQueue1.size(), 1);
    ASSERT_EQ(defQueue1.front()->getEventTypeIndex(), typeid(EventA));
}
}
