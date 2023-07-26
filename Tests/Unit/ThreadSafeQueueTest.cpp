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
    using QType = CppUtils::ThreadSafeQueue<NonCopyable>;

    QType q;
    const std::vector<int> values{1, 2, 3};
    std::thread t([&q, &values]
    {
        for (const auto value : values)
        {
            q.Push(value);
        }
        q.Push(QType::Terminate{});
    });

    REQUIRE(std::get<NonCopyable>(q.PopAndWait()) == values[0]);
    REQUIRE(std::get<NonCopyable>(q.PopAndWait()) == values[1]);
    REQUIRE(std::get<NonCopyable>(q.PopAndWait()) == values[2]);
    REQUIRE(QType::IsExit(q.PopAndWait()));
    t.join();
}
