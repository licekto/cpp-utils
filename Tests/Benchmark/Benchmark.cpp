#include <chrono>
#include <cmath>
#include <iostream>
#include <vector>

#include <CppUtils/ObjectPool.hpp>
#include <CppUtils/Utility.hpp>

#include "BoostObjectPool.hpp"
#include "DefaultAllocator.hpp"

namespace CppUtils::Benchmark
{
using IdT = uint32_t;

struct Person
{
    IdT id;
    std::string name, surname;
    uint8_t age;
};

template <typename T, template<typename> typename Allocator>
CppUtils::Stats measureObjectPool(const size_t capacity, const size_t iterations)
{
    using TimeUnit = std::chrono::microseconds;

    const auto times = CppUtils::measure<TimeUnit>(iterations, [&]()
    {
        Allocator<T> allocator(capacity);
        std::vector<T*> objects;

        for (size_t i = 0; i < capacity; ++i)
        {
            objects.push_back(allocator.Alloc());
        }

        for (auto *object : objects)
        {
            allocator.Free(object);
        }
    });

    return CppUtils::calculateStats(times);
}

template <typename T, template<typename> typename Allocator>
void measureObjectPool(const std::string& typeName, const std::string& allocatorName, const size_t capacity, const size_t iterations)
{
    const auto stats = measureObjectPool<T, Allocator>(capacity, iterations);
    std::cout << "iterations=" << iterations << ", mean=" << stats.mean << "us, standardDeviation=" << stats.standardDeviation << "us"
              << ", capacity=" << capacity << ", typeName=" << typeName << ", allocator=" << allocatorName << std::endl;
}

void objectPoolBenchmark()
{
    static constexpr size_t capacity = 10000;
    static constexpr size_t iterations = 1000;

    std::cout << "Object pool benchmark" << std::endl;

    measureObjectPool<uint64_t, DefaultAllocator>("uint64_t", "default(malloc)", capacity, iterations);
    measureObjectPool<uint64_t, CppUtils::ObjectPool>("uint64_t", "CppUtils::ObjectPool", capacity, iterations);

    measureObjectPool<Person, DefaultAllocator>("Person", "default(malloc)", capacity, iterations);
    measureObjectPool<Person, CppUtils::ObjectPool>("Person", "CppUtils::ObjectPool", capacity, iterations);

    static constexpr size_t boostCapacity = capacity / 100;
    static constexpr size_t boostIterations = iterations / 10;
    measureObjectPool<uint64_t, BoostObjectPool>("uint64_t", "BoostObjectPool", boostCapacity, boostIterations);
    measureObjectPool<Person, BoostObjectPool>("Person", "BoostObjectPool", boostCapacity, boostIterations);
}
}

int main()
{
    CppUtils::Benchmark::objectPoolBenchmark();
    return 0;
}
