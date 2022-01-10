#include <catch2/catch.hpp>

#include <CppUtils/Tokenizer.hpp>

#include <vector>

using namespace std::string_view_literals;

using DataT = std::vector<std::string_view>;
using DataItT = DataT::const_iterator;

template <typename ItT>
void testIteratorType(const ItT begin, const ItT end, const DataItT tBegin, const DataItT tEnd)
{
    auto tIt = tBegin;
    for (auto it = begin; it != end; ++it)
    {
        REQUIRE(*it == *tIt++);
    }
    REQUIRE(tIt == tEnd);
}

TEST_CASE("Syntax")
{
    using namespace std::string_view_literals;
    CppUtils::Tokenizer tokenizer("a -b1 - cxZ- dABZ"sv, "- "sv);
    const DataT tokens = {"a"sv, "b1"sv, "cxZ"sv, "dABZ"sv};

    const auto tBegin = std::cbegin(tokens), tEnd = std::cend(tokens);
    testIteratorType(std::begin(tokenizer), std::end(tokenizer), tBegin, tEnd);
    testIteratorType(std::cbegin(tokenizer), std::cend(tokenizer), tBegin, tEnd);

    auto it = std::cbegin(tokenizer);

    REQUIRE(it->size() == 1U);
    ++it;
    REQUIRE(it->size() == 2U);
    ++it;
    REQUIRE(it->size() == 3U);
    it++;
    REQUIRE(it->size() == 4U);
    it++;
    REQUIRE(it == std::cend(tokenizer));
}

TEST_CASE("Valid tokens")
{
    struct TestCase
    {
        std::string inputString;
        std::vector<std::string> expectedTokens;
        std::string delimiters = ", ";
    };

    const auto tc = GENERATE(TestCase{"", {}},
                             TestCase{" , ", {}},
                             TestCase{"a", {"a"}},
                             TestCase{",, , a   ,,, ", {"a"}},
                             TestCase{"a,b", {"a", "b"}},
                             TestCase{"a b", {"a", "b"}},
                             TestCase{"a,           b", {"a", "b"}},
                             TestCase{"a,           b        ", {"a", "b"}},
                             TestCase{"a            b   ,  ", {"a", "b"}},
                             TestCase{"    ,  a,    b   ,   ", {"a", "b"}},
                             TestCase{"ab, cd,  ef,    gh", {"ab", "cd", "ef", "gh"}},
                             TestCase{"1; 2;3;4", {"1", " 2", "3", "4"}, ";"},
                             TestCase{"1-2-3-4", {"1", "2", "3", "4"}, "-"},
                             TestCase{"1*-/2/3*4-5", {"1", "2", "3", "4", "5"}, "*-/"});

    auto expectedIt = std::begin(tc.expectedTokens);
    for (const auto token : CppUtils::Tokenizer(tc.inputString, tc.delimiters))
    {
        REQUIRE(token == *expectedIt++);
    }
    REQUIRE(expectedIt == std::end(tc.expectedTokens));
}

TEST_CASE("Invalid access")
{
    CppUtils::Tokenizer tokenizer("a -b1 - cxZ- dABZ"sv, "- "sv);
    auto it = std::end(tokenizer);
    REQUIRE_THROWS_WITH(++it, "Can't increment past end of the string 'a -b1 - cxZ- dABZ'");
    REQUIRE_THROWS_WITH(*it, Catch::Matchers::Contains("Can't access a token of the string 'a -b1 - cxZ- dABZ' at index="));
}
