#include <gtest/gtest.h>

#include "kc/core/Singleton.hpp"

class Singleton : public kc::core::Singleton<Singleton> {};

TEST(SingletonTests, givenSingleton_whenGettingInstance_shouldReturnValidAddress) {
    Singleton singleton;

    EXPECT_EQ(&singleton, &Singleton::get());
}

TEST(SingletonTests, givenSingleton_getPtrShouldReturnSameAddressAsGet) {
    Singleton singleton;

    EXPECT_EQ(&Singleton::get(), Singleton::getPtr());
}

TEST(SingletonTests, givenSingleton_whenCreatingSecondInstnace_shouldThrow) {
    Singleton singleton;

    EXPECT_THROW(Singleton{}, std::runtime_error);
}

TEST(SingletonTests, givenSingleSingletonWithoutInstance_whenGettingInstance_shouldReturnNullptr) {
    EXPECT_EQ(Singleton::getPtr(), nullptr);
}