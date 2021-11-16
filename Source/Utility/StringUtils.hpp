#pragma once

#include <string_view>
#include <sstream>

namespace CppUtils
{
namespace Detail
{
using WrongFuncType = char;
}

template <typename F = Detail::WrongFuncType>
class Combinator
{
public:
    Combinator(std::string _delimiter = "")
        : delimiter(std::move(_delimiter))
    {}

    Combinator(F&& _func)
        : func(std::move(_func))
    {}

    Combinator(const Combinator&) = delete;
    Combinator operator=(const Combinator&) = delete;

    template <typename First, typename Second, typename ...Tail>
    [[nodiscard]] std::string Combine(const First& first, const Second& second, const Tail&... tail) const &&
    {
        std::stringstream ss;
        ss << first << delimiter << second;
        ((ss << delimiter << tail), ...);
        return std::move(ss).str();
    }

    template <typename Container>
    [[nodiscard]] std::string Combine(const Container& container) const &&
    {
        std::stringstream ss;
        bool first = true;
        for (const auto& elem : container)
        {
            if (!first)
            {
                ss << delimiter;
            }
            ss << elem;
            first = false;
        }
        return std::move(ss).str();
    }

    template <typename Container>
    void Apply(Container& container)
    {
        static_assert (!std::is_same_v<F, Detail::WrongFuncType>, "Provide function to be applied");
        for (auto& elem : container)
        {
            func(elem);
        }
    }

private:
    std::string delimiter{};
    F func{};
};
}
