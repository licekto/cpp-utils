#pragma once

#include <boost/pool/object_pool.hpp>

#include <iostream>

namespace CppUtils::Benchmark
{

template <typename T>
class BoostObjectPool
{
public:
    BoostObjectPool(const size_t capacity)
        : pool(capacity)
    {}

    template <typename... Args>
    T *Alloc(Args&&... args)
    {
        return pool.construct(std::forward<Args>(args)...);
    }

    void Free(T *obj)
    {
        pool.destroy(obj);
    }

private:
    boost::object_pool<T> pool;
};
}
