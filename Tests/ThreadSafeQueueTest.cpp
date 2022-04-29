#include <catch2/catch.hpp>

#include <CppUtils/ThreadSafeQueue.hpp>

#include <thread>

struct NonCopyable
{
    NonCopyable(const int v)
        : val(v)
    {}
    operator int() const
    {
        return val;
    }
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&&) = default;
    NonCopyable& operator=(NonCopyable&&) = default;
    int val{};
};

TEST_CASE("ThreadSafeQueue test")
{
    ThreadSafeQueue<NonCopyable> q;
    const std::vector<int> values{1, 2, 3};
    std::thread t([&q, &values]
    {
        for (const auto value : values)
        {
            q.push(value);
        }
        q.push(Terminate{});
    });

    REQUIRE(std::get<NonCopyable>(q.popAndWait()) == values[0]);
    REQUIRE(std::get<NonCopyable>(q.popAndWait()) == values[1]);
    REQUIRE(std::get<NonCopyable>(q.popAndWait()) == values[2]);
    REQUIRE(isExit(q.popAndWait()));
    t.join();
}
