#include <catch2/catch.hpp>

#include <CppUtils/FullEnum.hpp>

TEST_CASE("Get until delimiter test", "[FullEnum]")
{
    const std::string expected = "abc";
    const std::string input = GENERATE("abc", "abc ", "abc,", "abc; ");
    std::stringstream ss;
    ss << input;
    const auto result = detail::getUntilDelim(ss);

    REQUIRE(result == expected);
}

DEF_ENUM(Day, mon, tue, wed, thu, fri, sat, sun);

TEST_CASE("FullEnum test", "[FullEnum]")
{
    Day d = Day::mon;

    REQUIRE(d.size() == 7);
    REQUIRE(d.toString() == std::string("mon"));

    d = Day::fromString("fri").value();

    REQUIRE(d == Day::fri);

    std::stringstream ss;
    ss << d;
    REQUIRE(ss.str() == "fri");

    ss.str("");
    ss << "wed";
    ss >> d;
    REQUIRE(d == Day::wed);

    Day d1 = Day::wed;

    REQUIRE(d == d1);

    REQUIRE(sizeof(d) <= 8);
}
