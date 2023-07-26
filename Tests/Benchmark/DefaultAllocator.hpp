#pragma once

#include <cstdint>
#include <utility>

namespace CppUtils::Benchmark
{
/**
 * @brief The DefaultAllocator class
 * @tparam T Object type for allocation
 *
 * Uses new/malloc and delete/free internally
 */
template <typename T>
struct DefaultAllocator
{
    DefaultAllocator(const std::size_t)
    {}

    template <typename... Args>
    T *Alloc(Args&&... args)
    {
        return new T(std::forward<Args>(args)...);
    }

    void Free(T *obj)
    {
        delete obj;
    }
};
}
