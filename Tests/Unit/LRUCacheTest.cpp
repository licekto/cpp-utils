#include <catch2/catch.hpp>

#include <iostream>
#include <sstream>
#include <thread>

#include <CppUtils/LRUCache.hpp>
#include <CppUtils/ObjectPool.hpp>
#include <CppUtils/Utility.hpp>

using T = uint32_t;


TEST_CASE("LRUCache - base cases", "[LRUCache]")
{
    CppUtils::LRUCache<T, T> lruCache(2);

    SECTION("Basic")
    {
        REQUIRE(lruCache.Get(0) == std::nullopt);
        REQUIRE(lruCache.Get(1) == std::nullopt);
        REQUIRE(lruCache.Get(2) == std::nullopt);
        REQUIRE(lruCache.Get(3) == std::nullopt);
        REQUIRE(lruCache.Get(4) == std::nullopt);

        lruCache.Insert(1, 1);
        REQUIRE(lruCache.Get(0) == std::nullopt);
        REQUIRE(lruCache.Get(1).value() == 1);
        REQUIRE(lruCache.Get(2) == std::nullopt);
        REQUIRE(lruCache.Get(3) == std::nullopt);
        REQUIRE(lruCache.Get(4) == std::nullopt);

        lruCache.Insert(2, 2);
        REQUIRE(lruCache.Get(0) == std::nullopt);
        REQUIRE(lruCache.Get(1).value() == 1);
        REQUIRE(lruCache.Get(2).value() == 2);
        REQUIRE(lruCache.Get(3) == std::nullopt);
        REQUIRE(lruCache.Get(4) == std::nullopt);

        lruCache.Insert(3, 3);
        REQUIRE(lruCache.Get(0) == std::nullopt);
        REQUIRE(lruCache.Get(1) == std::nullopt);
        REQUIRE(lruCache.Get(2).value() == 2);
        REQUIRE(lruCache.Get(3).value() == 3);
        REQUIRE(lruCache.Get(4) == std::nullopt);

        REQUIRE(lruCache.Get(2).value() == 2);

        lruCache.Insert(4, 4);
        REQUIRE(lruCache.Get(1) == std::nullopt);
        REQUIRE(lruCache.Get(2).value() == 2);
        REQUIRE(lruCache.Get(3) == std::nullopt);
        REQUIRE(lruCache.Get(4).value() == 4);
    }

    SECTION("Value overwrite")
    {
        lruCache.Insert(2, 2);
        lruCache.Insert(1, 1);
        lruCache.Insert(2, 22);
        lruCache.Insert(4, 4);
        REQUIRE(lruCache.Get(1) == std::nullopt);
        REQUIRE(lruCache.Get(2).value() == 22);
        REQUIRE(lruCache.Get(4).value() == 4);
    }
}

TEST_CASE("LRUCache - test cases", "[LRUCache]")
{
    CppUtils::LRUCache<T, T> lruCache(5);

    lruCache.Insert(0, 0);
    lruCache.Insert(1, 10);
    lruCache.Insert(2, 20);
    lruCache.Insert(3, 30);
    lruCache.Insert(4, 40);
    lruCache.Insert(5, 50);

    REQUIRE(lruCache.Get(0) == std::nullopt);
    REQUIRE(*lruCache.Get(5) == 50);
    REQUIRE(*lruCache.Get(4) == 40);
    REQUIRE(*lruCache.Get(3) == 30);
    REQUIRE(*lruCache.Get(2) == 20);
    REQUIRE(*lruCache.Get(1) == 10);
    REQUIRE(lruCache.Get(0) == std::nullopt);

    lruCache.Insert(1, 10);
    lruCache.Insert(2, 20);
    lruCache.Insert(3, 30);
    lruCache.Insert(4, 40);
    lruCache.Insert(5, 50);

    REQUIRE(lruCache.Get(0) == std::nullopt);
    REQUIRE(*lruCache.Get(5) == 50);
    REQUIRE(*lruCache.Get(4) == 40);
    REQUIRE(*lruCache.Get(3) == 30);
    REQUIRE(*lruCache.Get(2) == 20);
    REQUIRE(*lruCache.Get(1) == 10);
    REQUIRE(lruCache.Get(0) == std::nullopt);

    lruCache.Insert(6, 60);
    lruCache.Insert(7, 70);
    lruCache.Insert(8, 80);
    lruCache.Insert(9, 90);
    lruCache.Insert(10, 100);

    REQUIRE(lruCache.Get(0) == std::nullopt);
    REQUIRE(lruCache.Get(5) == std::nullopt);
    REQUIRE(lruCache.Get(4) == std::nullopt);
    REQUIRE(lruCache.Get(3) == std::nullopt);
    REQUIRE(lruCache.Get(2) == std::nullopt);
    REQUIRE(lruCache.Get(1) == std::nullopt);
    REQUIRE(lruCache.Get(0) == std::nullopt);

    REQUIRE(*lruCache.Get(10) == 100);
    REQUIRE(*lruCache.Get(9) == 90);
    REQUIRE(*lruCache.Get(8) == 80);
    REQUIRE(*lruCache.Get(7) == 70);
    REQUIRE(*lruCache.Get(6) == 60);

    REQUIRE(*lruCache.Get(6) == 60);
    REQUIRE(*lruCache.Get(7) == 70);
    REQUIRE(*lruCache.Get(8) == 80);
    REQUIRE(*lruCache.Get(9) == 90);
    REQUIRE(*lruCache.Get(10) == 100);

    REQUIRE(*lruCache.Get(8) == 80);

    lruCache.Insert(11, 110);
    lruCache.Insert(12, 120);
    lruCache.Insert(13, 130);

    REQUIRE(lruCache.Get(6) == std::nullopt);
    REQUIRE(lruCache.Get(7) == std::nullopt);
    REQUIRE(lruCache.Get(9) == std::nullopt);
    REQUIRE(*lruCache.Get(8) == 80);
    REQUIRE(*lruCache.Get(10) == 100);
    REQUIRE(*lruCache.Get(11) == 110);
    REQUIRE(*lruCache.Get(12) == 120);
    REQUIRE(*lruCache.Get(13) == 130);
}

TEST_CASE("LRUCache - randomized smoke test - single thread", "[LRUCache]")
{
    static constexpr T range = 10000;
    static constexpr size_t capacity = 100;

    std::unordered_map<T, T> testData;

    CppUtils::RandomDevice<T> rd(0, range);

    for (size_t i = 0; i < capacity; ++i)
    {
        while (1)
        {
            const auto key = rd.Get();
            if (testData.find(key) != std::end(testData))
            {
                continue;
            }
            testData[key] = rd.Get();
            break;
        }
    }

    CppUtils::LRUCache<T, T> lruCache(capacity);

    for (const auto& [key, value] : testData)
    {
        REQUIRE(lruCache.Get(key) == std::nullopt);
        lruCache.Insert(key, value);
        lruCache.Get(key);
        lruCache.Insert(key, value);
    }

    for (const auto& [key, value] : testData)
    {
        REQUIRE(*lruCache.Get(key) == testData[key]);
    }
}
