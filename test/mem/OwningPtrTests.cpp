#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace testing;

#include "kc/mem/allocators/Allocator.hpp"
#include "kc/mem/OwningPtr.hpp"

using namespace kc::mem;

template <typename T> struct MockAllocator : public Allocator<T> {
    MOCK_METHOD(T*, allocate, (std::size_t n), (override));
    MOCK_METHOD(void, deallocate, (T * ptr, std::size_t n), (noexcept, override));
};

struct Tester {
    Tester() { ++constructorCalls; }
    ~Tester() { ++destructorCalls; }

    inline static uint16_t constructorCalls = 0;
    inline static uint16_t destructorCalls  = 0;
};

struct OwningPtrTests : testing::Test {
    void SetUp() {
        Tester::constructorCalls = 0;
        Tester::destructorCalls  = 0;

        ON_CALL(allocator, allocate)
          .WillByDefault(Invoke([](std::size_t) -> Tester* {
              return static_cast<Tester*>(std::malloc(sizeof(Tester)));
          }));
        ON_CALL(allocator, deallocate)
          .WillByDefault(Invoke([](Tester* ptr, std::size_t) { std::free(ptr); }));
    }

    MockAllocator<Tester> allocator;
};

TEST_F(
  OwningPtrTests, givenPtr_whenCreatingAndDestroying_shouldAllocateAndDeallocate
) {
    EXPECT_CALL(allocator, allocate).Times(1);
    EXPECT_CALL(allocator, deallocate).Times(1);

    { auto ptr = makeOwningPtr<Tester>(&allocator); }
}

TEST_F(
  OwningPtrTests, givenDefaultConstructedPtr_whenGettingHandle_shouldReturnNullptr
) {
    OwningPtr<int> ptr;

    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_EQ(ptr.getAllocator(), nullptr);
}

TEST_F(OwningPtrTests, whenCreatingPointer_shouldStoreCorrectValue) {
    const int value = 1337;
    auto ptr        = makeOwningPtr<int>(value);

    EXPECT_EQ(*ptr, value);
    EXPECT_EQ(*ptr.get(), value);
}

TEST_F(OwningPtrTests, givenPtr_whenDestroying_shouldCallDestructorOfObject) {
    EXPECT_CALL(allocator, allocate).Times(1);
    EXPECT_CALL(allocator, deallocate).Times(1);

    ASSERT_EQ(Tester::constructorCalls, 0);
    ASSERT_EQ(Tester::destructorCalls, 0);

    {
        auto ptr = makeOwningPtr<Tester>(&allocator);
        EXPECT_EQ(Tester::constructorCalls, 1);
        EXPECT_EQ(Tester::destructorCalls, 0);
    }

    EXPECT_EQ(Tester::constructorCalls, 1);
    EXPECT_EQ(Tester::destructorCalls, 1);
}

TEST_F(
  OwningPtrTests, givenPtr_whenMoveConstructing_shouldNotDestructNorConstructTwice
) {
    EXPECT_CALL(allocator, allocate).Times(1);
    EXPECT_CALL(allocator, deallocate).Times(1);

    ASSERT_EQ(Tester::constructorCalls, 0);
    ASSERT_EQ(Tester::destructorCalls, 0);

    {
        OwningPtr<Tester> ptr;
        auto ptr2 = makeOwningPtr<Tester>(&allocator);

        ASSERT_EQ(Tester::constructorCalls, 1);
        ASSERT_EQ(Tester::destructorCalls, 0);

        ptr = std::move(ptr2);

        ASSERT_EQ(Tester::constructorCalls, 1);
        ASSERT_EQ(Tester::destructorCalls, 0);
    }

    ASSERT_EQ(Tester::constructorCalls, 1);
    ASSERT_EQ(Tester::destructorCalls, 1);
}

TEST_F(
  OwningPtrTests, givenPtr_whenMovingToAnotherPtr_shouldDestructTwiceAndCreateOnce
) {
    EXPECT_CALL(allocator, allocate).Times(2);
    EXPECT_CALL(allocator, deallocate).Times(2);

    ASSERT_EQ(Tester::constructorCalls, 0);
    ASSERT_EQ(Tester::destructorCalls, 0);

    {
        auto ptr  = makeOwningPtr<Tester>(&allocator);
        auto ptr2 = makeOwningPtr<Tester>(&allocator);

        ASSERT_EQ(Tester::constructorCalls, 2);
        ASSERT_EQ(Tester::destructorCalls, 0);

        ptr = std::move(ptr2);

        ASSERT_EQ(Tester::constructorCalls, 2);
        ASSERT_EQ(Tester::destructorCalls, 0);
    }

    ASSERT_EQ(Tester::constructorCalls, 2);
    ASSERT_EQ(Tester::destructorCalls, 1);
}
