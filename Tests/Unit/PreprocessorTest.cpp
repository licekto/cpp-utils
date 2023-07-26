#include <catch2/catch.hpp>

#include <CppUtils/Preprocessor.hpp>

TEST_CASE("VA_SIZE")
{
    REQUIRE(VA_SIZE() == 0);
    REQUIRE(VA_SIZE(a) == 1);
    REQUIRE(VA_SIZE(a, b) == 2);
    REQUIRE(VA_SIZE(a, c, d) == 3);
    REQUIRE(VA_SIZE(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20) == 20);
}

TEST_CASE("FOR_EACH")
{
    const char *arr[] = {TO_STRINGS(ab, cd, ef)};

    REQUIRE(arr[0][0] == 'a');
    REQUIRE(arr[0][1] == 'b');
    REQUIRE(arr[1][0] == 'c');
    REQUIRE(arr[1][1] == 'd');
    REQUIRE(arr[2][0] == 'e');
    REQUIRE(arr[2][1] == 'f');
}
