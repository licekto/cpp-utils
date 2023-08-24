#include <catch2/catch.hpp>

#include <cstdint>
#include <vector>

#include <CppUtils/ObjectPool.hpp>
#include <CppUtils/Utility.hpp>

TEST_CASE("ObjectPool - base cases", "[ObjectPool]")
{
    static constexpr size_t capacity = 5;
    CppUtils::ObjectPool<uint32_t> objectPool(capacity);

    std::vector<uint32_t*> objs;

    for (size_t i = 0; i < 5; ++i)
    {
        auto obj = objectPool.Alloc();
        objs.push_back(obj);
        *obj = i;
    }

    using Catch::Matchers::Contains;
    REQUIRE_THROWS_WITH(objectPool.Alloc(), "The object pool's capacity=" + std::to_string(capacity) + " exceeded");

    for (size_t i = 0; i < 5; ++i)
    {
        auto obj = objs[i];
        REQUIRE(*obj == i);
        objectPool.Free(obj);
        REQUIRE_THROWS_WITH(objectPool.Free(obj), Contains("The object at address '") && Contains("' has already been freed"));
    }

    REQUIRE_THROWS_WITH(objectPool.Free(nullptr), "The object at address '0' is not managed by this object pool");

    uint32_t *ptr = new uint32_t;
    REQUIRE_THROWS_WITH(objectPool.Free(ptr), Contains("The object at address '") && Contains("' is not managed by this object pool"));
    delete ptr;
}

TEST_CASE("ObjectPool - randomized smoke test", "[ObjectPool]")
{
    struct Person
    {
        std::string name, surname;
        uint32_t age;
        std::vector<std::string> skills;
    };

    static constexpr size_t capacity = 10000;
    CppUtils::ObjectPool<Person> objectPool(capacity);
    std::vector<Person *> persons;

    static constexpr size_t range = capacity - 1;
    static constexpr size_t iterations = 1000;
    CppUtils::RandomDevice<size_t> rd(0, range);

    for (size_t i = 0; i < capacity; ++i)
    {
        persons.push_back(objectPool.Alloc());
    }

    for (size_t i = 0; i < iterations; ++i)
    {
        try
        {
            objectPool.Free(persons[rd.Get()]);
        }
        catch (const CppUtils::ObjectPoolDoubleFreeException&)
        {
            // ignore
        }
    }

    for (size_t i = 0; i < capacity; ++i)
    {
        try
        {
            persons.push_back(objectPool.Alloc());
        }
        catch (const CppUtils::ObjectPoolCapacityException&)
        {
            // ignore
        }
    }

    for (auto *obj : persons)
    {
        try
        {
            objectPool.Free(obj);
        }
        catch (const CppUtils::ObjectPoolDoubleFreeException&)
        {
            // ignore
        }
    }

    REQUIRE(objectPool.FreeFrames() == capacity);
}

struct TestType
{
    ~TestType()
    {
        ++destructorCalled;
    }
    static inline size_t destructorCalled = 0;
};

TEST_CASE("ObjectPool - type destructor call test", "[ObjectPool]")
{
    CppUtils::ObjectPool<TestType> objectPool(3);

    REQUIRE(TestType::destructorCalled == 0);

    auto obj1 = objectPool.Alloc();
    auto obj2 = objectPool.Alloc();
    auto obj3 = objectPool.Alloc();

    REQUIRE(TestType::destructorCalled == 0);

    objectPool.Free(obj1);
    REQUIRE(TestType::destructorCalled == 1);

    objectPool.Free(obj2);
    REQUIRE(TestType::destructorCalled == 2);

    objectPool.Free(obj3);
    REQUIRE(TestType::destructorCalled == 3);
}

TEST_CASE("ObjectPool - variadic constructor", "[ObjectPool]")
{
    struct X
    {
        double val;
        std::string str;

        X(const double val, const std::string& str)
            : val(val)
            , str(str)
        {}
    };

    CppUtils::ObjectPool<X> objectPool(3);

    objectPool.Alloc(3.14, std::string("PI"));
}
