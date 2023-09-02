#pragma once

#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <vector>

namespace CppUtils
{

struct Stats
{
    const double mean{0}, standardDeviation{0};
    std::string ToString(const std::string& units) const;
};

template <typename UnitT>
Stats calculateStats(const std::vector<UnitT>& measuredTimes)
{
    double sum = 0.0;

    for (const auto& t : measuredTimes)
    {
        sum += t.count();
    }

    double mean = sum / measuredTimes.size();

    double standardDeviation = 0.0;

    for (const auto& t : measuredTimes)
    {
        standardDeviation += std::pow(t.count() - mean, 2);
    }

    standardDeviation = std::sqrt(standardDeviation / measuredTimes.size());

    return {mean, standardDeviation};
}

template <typename UnitT, typename F>
auto measure(const size_t iterations, F&& f)
{
    std::vector<std::chrono::microseconds> measuredTimes;

    for (size_t i = 0; i < iterations; ++i)
    {
        const auto start = std::chrono::high_resolution_clock::now();
        std::forward<F>(f)();
        const auto end = std::chrono::high_resolution_clock::now();
        measuredTimes.push_back(std::chrono::duration_cast<UnitT>(end - start));
    }

    return measuredTimes;
}

template <typename UnitT>
class Profiler
{
public:
    using DestructorCallback = std::function<void(UnitT)>;
    Profiler(DestructorCallback&& destructorCallback = {})
        : destructorCallback(std::move(destructorCallback))
        , begin(std::chrono::high_resolution_clock::now())
    {}

    ~Profiler()
    {
        const auto end = std::chrono::high_resolution_clock::now();
        if (destructorCallback)
        {
            destructorCallback(std::chrono::duration_cast<UnitT>(end - begin));
        }
    }

    void Restart()
    {
        begin = std::chrono::high_resolution_clock::now();
    }

    size_t Measure()
    {
        const auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<UnitT>(end - begin).count();
    }

private:
    DestructorCallback destructorCallback;
    std::chrono::time_point<std::chrono::high_resolution_clock> begin;
};

}
