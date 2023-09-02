#include <iostream>
#include <thread>
#include <unordered_map>
#include <vector>

#include <CppUtils/LRUCache.hpp>
#include <CppUtils/ObjectPool.hpp>
#include <CppUtils/Random.hpp>

#ifndef CAPACITY
    #define CAPACITY 5000
#endif

#ifndef THREADS_MAX
    #define THREADS_MAX 10
#endif

using T = uint32_t;
using ThreadData = std::unordered_map<T, T>;
static constexpr T range = 10000;
static constexpr size_t capacityMax = CAPACITY;
static constexpr size_t threadsMax = THREADS_MAX;

using LRUCacheT = CppUtils::LRUCache<T, T>;

void correctnessTest()
{
    CppUtils::RandomDevice<T> rd(0, range);

    for (size_t threadsCount = 2; threadsCount < threadsMax; ++threadsCount)
    {
        const size_t capacity = capacityMax / threadsCount;

        std::vector<ThreadData> testData;

        for (size_t i = 0; i < threadsCount; ++i)
        {
            ThreadData threadData;
            for (size_t j = 0; j < capacity; ++j)
            {
                while (1)
                {
                    const auto key = rd.Get();
                    if (threadData.find(key) != std::end(threadData))
                    {
                        continue;
                    }
                    threadData[key] = rd.Get();
                    break;
                }
            }
            testData.push_back(threadData);
        }

        LRUCacheT lruCache(capacityMax);

        std::vector<std::thread> threads;

        for (size_t i = 0; i < threadsCount; ++i)
        {
            threads.push_back(std::thread([&lruCache, threadData = testData[i]]()
            {
                for (const auto& [key, value] : threadData)
                {
                  lruCache.Insert(key, value);
                  lruCache.Get(key);
                  lruCache.Insert(key, value);
                }

                for (const auto& [key, value] : threadData)
                {
                  lruCache.Get(key);
                }
            }));
        }

        for (auto& thread : threads)
        {
            thread.join();
        }

        for (const auto& threadData : testData)
        {
            for (const auto& [key, value] : threadData)
            {
                if (lruCache.Get(key) == std::nullopt)
                {
                    throw std::runtime_error("The key '" + std::to_string(key) + "' could not be found");
                }
                const auto cachedValue = *lruCache.Get(key);
                bool found = false;
                for (const auto& threadData : testData)
                {
                    const auto it = threadData.find(key);
                    if (it != std::end(threadData) && it->second == cachedValue)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    throw std::runtime_error("The cached value '" + std::to_string(cachedValue) + "' could not be found");
                }
            }
        }
    }
}

void smokeTest()
{
    CppUtils::RandomDevice<T> rd(0, range);
    LRUCacheT lruCache(capacityMax);
    std::vector<std::thread> threads;
    std::vector<T> values;

    const size_t iterations = 5;

    for (size_t i = 0; i < threadsMax; ++i)
    {
        threads.push_back(std::thread([&lruCache, &rd]()
        {
            for (size_t i = 0; i < iterations; ++i)
            {
                for (size_t j = 0; j < capacityMax; ++j)
                {
                    const auto someKey = rd.Get();
                    lruCache.Get(someKey);
                    lruCache.Insert(someKey, rd.Get());
                    lruCache.Get(someKey);
                    lruCache.Insert(someKey, rd.Get());
                    lruCache.Insert(rd.Get(), rd.Get());
                    lruCache.Get(someKey);
                    lruCache.Get(someKey);
                    lruCache.Get(rd.Get());
                }
            }
        }));
    }

    for (auto& thread : threads)
    {
        thread.join();
    }
}

int main()
{
    std::cout << "capacity=" << capacityMax << ", threadsMax=" << threadsMax << std::endl;
    correctnessTest();
    smokeTest();
    return 0;
}
