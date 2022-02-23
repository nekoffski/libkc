#include <gtest/gtest.h>

#include "kc/event/EventListener.h"

namespace {

const std::string ident = "concreteIdent123";

class ConcreteEventListener : public kc::event::EventListener {
   public:
    ConcreteEventListener() : kc::event::EventListener(ident) {}
    void handleEvents(const kc::event::EventProvider&) {}
};

TEST(EventListenerTests, givenEventListener_whenGettingIdent_expectCorrectIdent) {
    ASSERT_EQ(ident, ConcreteEventListener{}.getIdent());
}
}  // namespace
