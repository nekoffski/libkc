#include <gtest/gtest.h>

#include "kc/event/EventProvider.h"

#define TYPE(t) \
    std::type_index { typeid(t) }

struct CategoryA : kc::event::Category { };
struct CategoryB : kc::event::Category { };
struct CategoryC : kc::event::Category { };

namespace {

TEST(ExtractorTests, whenExtractingTypes_shouldReturnProperTypeIds) {
    kc::event::detail::TypeVector categories;
    kc::event::detail::Extractor<int, float, bool>::extract(categories);

    EXPECT_EQ(categories[0], TYPE(int));
    EXPECT_EQ(categories[1], TYPE(float));
    EXPECT_EQ(categories[2], TYPE(bool));
}

TEST(ExtractorTests, whenExtractingCategories_shouldReturnProperTypeIds) {
    kc::event::detail::TypeVector categories;
    kc::event::detail::Extractor<CategoryA, CategoryB>::extract(categories);

    EXPECT_EQ(categories[0], TYPE(CategoryA));
    EXPECT_EQ(categories[1], TYPE(CategoryB));
}

struct EventA : kc::event::EventBase<EventA> { };
struct EventB : kc::event::EventBase<EventB, CategoryA> { };
struct EventC : kc::event::EventBase<EventC, CategoryB> { };

class EventProviderTests : public testing::Test {
protected:
    explicit EventProviderTests()
        : m_eventProvider(m_events) {
    }

    void SetUp() override {
        m_eventA = std::make_shared<EventA>();
        m_eventB = std::make_shared<EventB>();
        m_eventC = std::make_shared<EventC>();

        m_events[m_eventA->getCategoryTypeIndex()].push_back(m_eventA);
        m_events[m_eventB->getCategoryTypeIndex()].push_back(m_eventB);
        m_events[m_eventC->getCategoryTypeIndex()].push_back(m_eventC);
    }

    kc::event::CategoryToEventQueue m_events;
    kc::event::EventProvider m_eventProvider;

    std::shared_ptr<EventA> m_eventA;
    std::shared_ptr<EventB> m_eventB;
    std::shared_ptr<EventC> m_eventC;

    bool checkEvent(std::shared_ptr<kc::event::Event> event, const kc::event::EventQueue& q) {
        for (auto& e : q)
            if (event->getEventTypeIndex() == e->getEventTypeIndex())
                return true;
        return false;
    }
};

TEST_F(EventProviderTests, givenThreeEvents_whenGettingAll_shouldReturnAllEvents) {
    auto events = m_eventProvider.getAll();

    ASSERT_EQ(events.size(), 3);
    ASSERT_TRUE(checkEvent(m_eventA, events));
    ASSERT_TRUE(checkEvent(m_eventB, events));
    ASSERT_TRUE(checkEvent(m_eventC, events));
}

TEST_F(EventProviderTests, givenThreeEvents_whenGettingEventByCategory_shouldReturnOnlyThisEvent) {
    auto events1 = m_eventProvider.getByCategories<kc::event::DefaultCategory>();
    ASSERT_EQ(events1.size(), 1);
    ASSERT_TRUE(checkEvent(m_eventA, events1));
    ASSERT_FALSE(checkEvent(m_eventB, events1));
    ASSERT_FALSE(checkEvent(m_eventC, events1));

    auto events2 = m_eventProvider.getByCategories<CategoryA>();
    ASSERT_EQ(events2.size(), 1);
    ASSERT_FALSE(checkEvent(m_eventA, events2));
    ASSERT_TRUE(checkEvent(m_eventB, events2));
    ASSERT_FALSE(checkEvent(m_eventC, events2));

    auto events3 = m_eventProvider.getByCategories<CategoryB>();
    ASSERT_EQ(events3.size(), 1);
    ASSERT_FALSE(checkEvent(m_eventA, events3));
    ASSERT_FALSE(checkEvent(m_eventB, events3));
    ASSERT_TRUE(checkEvent(m_eventC, events3));

    ASSERT_EQ(m_eventProvider.getByCategories<CategoryC>().size(), 0);
}

TEST_F(EventProviderTests, givenThreeEvents_whenGettingEventByTwoCategories_shouldReturnThisTwoEvents) {
    auto events = m_eventProvider.getByCategories<CategoryA, CategoryB>();
    ASSERT_EQ(events.size(), 2);
    ASSERT_FALSE(checkEvent(m_eventA, events));
    ASSERT_TRUE(checkEvent(m_eventB, events));
    ASSERT_TRUE(checkEvent(m_eventC, events));
}
}
