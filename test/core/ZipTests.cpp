#include "kc/core/Zip.hpp"

#include <gtest/gtest.h>

using namespace kc;

TEST(ZipTests, givenTwoContainers_whenIteratingOverWithZip_shouldCorrectlyIterateOver) {
    std::vector<int> a = { 1, 2, 3, 4, 5 };
    std::vector<int> b = { 0, 0, 0, 0, 0 };

    for (auto&& [al, bl] : core::zip(a, b))
        al = bl;

    for (int i = 0; i < a.size(); ++i)
        EXPECT_EQ(a[i], b[i]);
}

TEST(ZipTests, givenThreeContainers_whenIteratingOverWithZip_shouldCorrectlyIterateOver) {
    std::vector<int> a = { 1, 2, 3, 4, 5 };
    std::vector<int> b = { 0, 0, 0, 0, 0 };
    std::vector<int> c = { 1, 2, 3, 4, 5 };

    for (auto&& [al, bl, cl] : core::zip(a, b, c))
        bl = al + cl;

    for (int i = 0; i < a.size(); ++i)
        EXPECT_EQ(b[i], a[i] + c[i]);
}