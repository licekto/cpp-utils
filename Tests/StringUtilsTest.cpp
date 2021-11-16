#include <catch2/catch.hpp>

#include <StringUtils.hpp>

TEST_CASE("StringUtils Combinator Combine")
{
    REQUIRE("1, 2, sTR#, 3.14, nullptr" == CppUtils::Combinator(", ").Combine(1, '2', "sTR#", 3.14, nullptr));
}

TEST_CASE("StringUtils Combinator Container")
{
    std::vector<int> vec{1, 2, 3, 4};
    REQUIRE("1;2;3;4" == CppUtils::Combinator(";").Combine(vec));

    CppUtils::Combinator([](auto& elem){ elem *= 2; }).Apply(vec);
    REQUIRE(2 == vec[0]);
    REQUIRE(4 == vec[1]);
    REQUIRE(6 == vec[2]);
    REQUIRE(8 == vec[3]);
}
