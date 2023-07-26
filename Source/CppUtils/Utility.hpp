#pragma once

#include <chrono>
#include <cmath>
#include <limits>
#include <mutex>
#include <random>
#include <vector>

namespace CppUtils
{
struct Stats
{
    const double mean{0}, standardDeviation{0};
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

template <typename T = uint32_t>
class RandomDevice
{
public:
    RandomDevice(const T from, const T to);
    T Get();

private:
    std::mutex mutex;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<T> distribution;
};

template <typename T>
RandomDevice<T>::RandomDevice(const T from, const T to)
    : gen(rd())
    , distribution(from, to)
{}

template <typename T>
T RandomDevice<T>::Get()
{
    std::unique_lock lock(mutex);
    return distribution(gen);
}
}
