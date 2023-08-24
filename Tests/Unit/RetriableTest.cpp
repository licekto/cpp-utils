#include <catch2/catch.hpp>

#include <CppUtils/Retriable.hpp>

using namespace std::chrono_literals;

static constexpr auto period = 30ms;
static constexpr auto delay = period + 20ms;
static constexpr uint32_t retries = 5;

TEST_CASE("Immediate success", "[Retriable]")
{
    auto f = [](const double a, const std::string& str) -> int
    {
        return static_cast<int>(a) + static_cast<int>(str.size());
    };

    CppUtils::Retriable<int, const double, const std::string&> retriable(period, retries, f);
    const auto result = retriable.Run(10.5, "abc");

    REQUIRE(13 == result);
}

TEST_CASE("Successful retry after delay", "[Retriable]")
{
    static constexpr uint32_t delayedTimes = 3;

    struct DelayedTask
    {
        DelayedTask(std::atomic<uint32_t>& attempts_)
            : attempts(attempts_)
        {}

        std::string operator()(const int a, const int b)
        {
            ++attempts;
            if (attempts < delayedTimes)
            {
                std::this_thread::sleep_for(delay);
            }
            return std::to_string(a) + "-" + std::to_string(b);
        }

        std::atomic<uint32_t>& attempts;
    };

    std::atomic<uint32_t> attempts = 0;
    DelayedTask task(attempts);

    CppUtils::Retriable<std::string, const int, const int> retriable(period, retries, task);
    const auto result = retriable.Run(1, 50);

    REQUIRE("1-50" == result);
    REQUIRE(delayedTimes == attempts);
}

static constexpr uint32_t defaultValue = 123445566;
TEST_CASE("Delay too long", "[Retriable]")
{
    struct X
    {
        uint32_t value = defaultValue;
    };

    std::atomic<uint32_t> attempts = 0;
    auto f = [&attempts](const uint32_t a, const uint32_t b)
    {
        ++attempts;
        std::this_thread::sleep_for(delay);
        return X{ a + b };
    };

    CppUtils::Retriable<X, const uint32_t, const uint32_t> retriable(period, retries, f);

    REQUIRE_THROWS_AS(retriable.Run(1, 2), std::invalid_argument);
    REQUIRE(retries == attempts);
}

struct TestException : public std::exception
{
    const char* what() const noexcept
    {
        return "msg";
    }
};

TEST_CASE("Exception after detach", "[Retriable]")
{
    std::atomic<uint32_t> attempts = 0;
    auto f = [&attempts]() -> uint32_t
    {
        ++attempts;
        if (attempts < 2)
        {
            std::this_thread::sleep_for(delay);
            throw TestException();
        }
        return 0;
    };

    CppUtils::Retriable<uint32_t> retriable(period, retries, f);
    REQUIRE_NOTHROW(retriable.Run());
}

TEST_CASE("Exception before detach", "[Retriable]")
{
    auto f = []() -> uint32_t
    {
        throw TestException();
    };

    CppUtils::Retriable<uint32_t> retriable(period, retries, f);
    REQUIRE_THROWS_AS(retriable.Run(), TestException);
}

TEST_CASE("Exception before detach, multiple attempts", "[Retriable]")
{
    std::atomic<uint32_t> attempt = 0;
    auto f = [&attempt]() -> uint32_t
    {
        ++attempt;
        if (attempt >= 3)
        {
            throw TestException();
        }
        std::this_thread::sleep_for(delay);
        return 0;
    };

    CppUtils::Retriable<uint32_t> retriable(period, retries, f);
    REQUIRE_THROWS_AS(retriable.Run(), TestException);
}

TEST_CASE("Limits", "[Retriable]")
{
    auto f = []() -> uint32_t
    {
        return 0;
    };

    const uint32_t retries = GENERATE(0, 10);
    bool exceptionThrown{false};
    try
    {
        CppUtils::Retriable<uint32_t> retriable1(period, retries, f);
    }
    catch (const std::invalid_argument& ex)
    {
        exceptionThrown = true;
    }
    REQUIRE(exceptionThrown);
}

TEST_CASE("Copy/move", "[Retriable]")
{
    static constexpr uint32_t delayedTimes = 2;

    struct DelayedTask
    {
        DelayedTask(std::atomic<uint32_t>& attempts_)
            : attempts(attempts_)
        {}

        std::string operator()(std::string value)
        {
            ++attempts;
            if (attempts < delayedTimes)
            {
                std::this_thread::sleep_for(delay);
            }
            return value;
        }

        std::atomic<uint32_t>& attempts;
    };

    std::atomic<uint32_t> attempts = 0;
    DelayedTask task(attempts);

    CppUtils::Retriable<std::string, std::string> retriable(period, retries, task);
    const auto result = retriable.Run("value");

    REQUIRE("value" == result);
    REQUIRE(delayedTimes == attempts);
}
